import re
import glob
import os
from collections import defaultdict
import pymorphy2
from natasha import Segmenter, MorphVocab, NewsEmbedding, NewsMorphTagger, NewsSyntaxParser, Doc
from natasha.ner import NewsNERTagger

# Инициализируем морфологический анализатор pymorphy2
morph = pymorphy2.MorphAnalyzer()


def to_nominative(text):
    """
    Приводит каждое слово в строке к именительному падежу.
    Для имен (PER) и локаций (LOC) это позволит получить базовую (номинативную) форму.
    """
    words = text.split()
    nominative_words = []
    for word in words:
        p = morph.parse(word)[0]
        inflected = p.inflect({'nomn'})
        if inflected:
            nominative_words.append(inflected.word)
        else:
            nominative_words.append(word)
    return " ".join(nominative_words)


def get_tense(token):
    """
    Извлекает информацию о времени из морфологических признаков токена.
    Если в token.feats содержится Tense=Past или Tense=Pres, то возвращает
    «прошедшее» или «настоящее» соответственно.
    """
    if token.feats and isinstance(token.feats, dict):
        tense = token.feats.get("Tense")
        if tense:
            if tense == "Past":
                return "прошедшее"
            elif tense == "Pres":
                return "настоящее"
            else:
                return "будущее"
    return ""


def clean_text(text):
    """
    Удаляет служебные строки (например, заголовки Project Gutenberg) из текста.
    """
    text = re.sub(r'\*\*\* START OF.*\*\*\*', '', text)
    text = re.sub(r'\*\*\* END OF.*\*\*\*', '', text)
    return text


def analyze_text(text):
    """
    Обрабатывает текст с помощью Natasha:
      - Извлекает именованные сущности (doc.spans)
      - Составляет пары:
          1. <Персонаж>:<Локация> – по совместному появлению (приводим PER и LOC к именительному падежу)
          2. <Локация>:<Организация> – по совместному появлению (для LOC приводим к именительному падежу)
          3. <Персонаж>:<Действие> – на основе синтаксического анализа: если глагол имеет подлежащее,
             попадающее в сущность типа PER, добавляем время действия
          4. <Организация>:<Действие> – аналогично для сущностей типа ORG (без преобразования в именительный падеж)
    """
    segmenter = Segmenter()
    emb = NewsEmbedding()
    morph_tagger = NewsMorphTagger(emb)
    syntax_parser = NewsSyntaxParser(emb)
    ner_tagger = NewsNERTagger(emb)

    doc = Doc(text)
    doc.segment(segmenter)
    doc.tag_morph(morph_tagger)
    doc.parse_syntax(syntax_parser)
    doc.tag_ner(ner_tagger)

    # Словари для подсчета пар
    per_loc = defaultdict(int)  # <Персонаж>:<Локация>
    loc_org = defaultdict(int)  # <Локация>:<Организация>
    per_action = defaultdict(int)  # <Персонаж>:<Действие> с временем
    org_action = defaultdict(int)  # <Организация>:<Действие> с временем

    spans = doc.spans
    n = len(spans)
    # Извлечение пар по совместному появлению сущностей
    for i in range(n):
        for j in range(i + 1, n):
            span1 = spans[i]
            span2 = spans[j]
            # Пары <Персонаж>:<Локация>
            if span1.type == 'PER' and span2.type == 'LOC':
                per = to_nominative(span1.text)
                loc = to_nominative(span2.text)
                per_loc[(per, loc)] += 1
            elif span1.type == 'LOC' and span2.type == 'PER':
                per = to_nominative(span2.text)
                loc = to_nominative(span1.text)
                per_loc[(per, loc)] += 1
            # Пары <Локация>:<Организация>
            if span1.type == 'LOC' and span2.type == 'ORG':
                loc = to_nominative(span1.text)
                org = span2.text
                loc_org[(loc, org)] += 1
            elif span1.type == 'ORG' and span2.type == 'LOC':
                loc = to_nominative(span2.text)
                org = span1.text
                loc_org[(loc, org)] += 1

    # Извлечение пар "сущность:действие" на основе синтаксического анализа.
    # Для каждого глагола (VERB) ищем его подлежащие (rel == 'nsubj').
    for token in doc.tokens:
        if token.pos == 'VERB':
            tense = get_tense(token)
            subjects = [t for t in doc.tokens if t.head_id == token.id and t.rel == 'nsubj']
            for subj in subjects:
                for span in doc.spans:
                    if span.start <= subj.start < span.stop:
                        if span.type == 'PER':
                            per = to_nominative(span.text)
                            # Ключ – тройка: (Персонаж, Действие, время)
                            per_action[(per, token.text, tense)] += 1
                        elif span.type == 'ORG':
                            org_action[(span.text, token.text, tense)] += 1
    return per_loc, per_action, loc_org, org_action


def process_files(input_folder, output_file):
    """
    Обрабатывает все файлы с расширением .txt из папки input_folder, агрегирует результаты
    и записывает их в файл output_file.
    """
    overall_per_loc = defaultdict(int)
    overall_per_action = defaultdict(int)
    overall_loc_org = defaultdict(int)
    overall_org_action = defaultdict(int)

    txt_files = glob.glob(os.path.join(input_folder, "*.txt"))

    for file_path in txt_files:
        with open(file_path, 'r', encoding='utf-8') as f:
            text = f.read()
        text = clean_text(text)
        per_loc, per_action, loc_org, org_action = analyze_text(text)
        for pair, count in per_loc.items():
            overall_per_loc[pair] += count
        for pair, count in per_action.items():
            overall_per_action[pair] += count
        for pair, count in loc_org.items():
            overall_loc_org[pair] += count
        for pair, count in org_action.items():
            overall_org_action[pair] += count

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("Результаты анализа:\n\n")

        f.write("1. Пары <Персонаж>:<Локация>\n")
        f.write("Персонаж : Локация\tЧастота\n")
        for pair, freq in sorted(overall_per_loc.items(), key=lambda x: -x[1]):
            f.write(f"{pair[0]} : {pair[1]}\t{freq}\n")

        f.write("\n2. Пары <Персонаж>:<Действие>\n")
        f.write("Персонаж : Действие (время)\tЧастота\n")
        for pair, freq in sorted(overall_per_action.items(), key=lambda x: -x[1]):
            # pair имеет вид (персонаж, действие, tense)
            tense_str = f" ({pair[2]})" if pair[2] else ""
            f.write(f"{pair[0]} : {pair[1]}{tense_str}\t{freq}\n")

        f.write("\n3. Пары <Локация>:<Организация>\n")
        f.write("Локация : Организация\tЧастота\n")
        for pair, freq in sorted(overall_loc_org.items(), key=lambda x: -x[1]):
            f.write(f"{pair[0]} : {pair[1]}\t{freq}\n")

        f.write("\n4. Пары <Организация>:<Действие>\n")
        f.write("Организация : Действие (время)\tЧастота\n")
        for pair, freq in sorted(overall_org_action.items(), key=lambda x: -x[1]):
            tense_str = f" ({pair[2]})" if pair[2] else ""
            f.write(f"{pair[0]} : {pair[1]}{tense_str}\t{freq}\n")


if __name__ == '__main__':
    # Укажите папку с входными текстовыми файлами (например, "texts")
    input_folder = "texts"
    # Имя выходного файла для записи результатов анализа
    output_file = "analysis_results.txt"
    process_files(input_folder, output_file)
