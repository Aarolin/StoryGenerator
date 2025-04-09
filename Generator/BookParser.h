#pragma once

#include "Word.h"
#include <cwctype>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

/// <summary>
/// Класс для непосредственного управления чтением
/// в файле. Выполняет разбиение строки, хранит флаги 
/// конца файла и зоны словесной категории.
/// </summary>
class BookParser {
public:

	/// <summary>
	/// Запись с категориями. Содержит названия категорий для предстоящих пар слов.
	/// </summary>
	struct ParsedCategoryRecord {
		WordCategory leftWordsCategory = WordCategory::NONE;
		WordCategory rightWordsCategory = WordCategory::NONE;
	};

	/// <summary>
	/// Запись файла со словами и частотой. В каждой строке содержится два слова.
	/// Одно из них может быть составным, например имя фамилия.
	/// Флаг success отвечает за успешное чтение строки файла.
	/// Дополнительно хранятся времена для каждого слова.
	/// </summary>
	struct ParsedRecord {
		std::wstring firstWord;
		std::wstring secondWord;
		int frequency;
		WordTense firstWordTense = WordTense::PRESENT;
		WordTense secondWordTense = WordTense::PRESENT;
		bool success = false;
	};
	
	/// <summary>
	/// Конструктор по умолчанию.
	/// </summary>
	BookParser();
	
	/// <summary>
	/// Устанавливает путь к файлу для потока чтения fileReader_.
	/// </summary>
	/// <param name="p">Объект, содержащий путь к файлу чтения.</param>
	void openFile(const std::filesystem::path p);

	/// <summary>
	/// Отслеживает флаг конца файла.
	/// </summary>
	/// <returns>Возвращает флаг eof файлового потока.</returns>
	bool eof() const;

	/// <summary>
	/// Возвращает флаг конца категории пар слов.
	/// </summary>
	/// <returns></returns>
	bool isEndOfCategory() const;
	
	/// <summary>
	/// Считывает строку с предстоящими категориями пар слов.
	/// </summary>
	/// <returns>Структурированная запись с предстоящими категориями слов.</returns>
	ParsedCategoryRecord readCategoryLine();

	/// <summary>
	/// // Считывает строку файла с парами слов и частотой.
	/// </summary>
	/// <param name="leftWordsCategory">Категория первого слова</param>
	/// <param name="rightWordsCategory">Категория второго слова</param>
	/// <returns>Структурированная запись файла со словами.</returns>
	ParsedRecord readWordsLine(const WordCategory& leftWordsCategory, const WordCategory& rightWordsCategory);

	/// <summary>
	/// После окончания чтения пар слов в текущей категории
	/// переключает флаг endOfCategory_ в состояние false, чтобы
	/// начать чтение следующей категории или завершить чтение файла.
	/// </summary>
	void setNextCategory();

private:

	/// <summary>
	/// Поток чтения символов юникод.
	/// </summary>
	std::wistringstream buffer_;

	/// <summary>
	/// Поток чтения файлов с символами юникод.
	/// </summary>
	std::wifstream fileReader_;

	// В строке файла со словами символ ':' является разделителем
	// между словами.
	wchar_t wordSeparator_ = ':';

	/// <summary>
	/// В строке файла с категориями предстоящих пар слов
	/// символ '<' является началом названия категории.
	/// </summary>
	wchar_t wordCategoryStartCh_ = '<';

	// В строке файла с категориями предстоящих пар слов
	// символ '>' является концом названия категории.
	wchar_t wordCategoryEndCh_ = '>';

	/// <summary>
	/// Флаг для отслеживания конца зоны с текущей категорией слов. 
	/// </summary>
	bool endOfCategory_ = false;

	/// <summary>
	/// Флаг для отслеживания зоны, в которой находятся слова.
	/// </summary>
	bool isWordsArea_ = false;

	/// <summary>
	/// Считывает строку со словами. 
	/// </summary>
	/// <returns>Возвращает true при успешном чтении строки слов</returns>
	bool readNextLine();

	/// <summary>
	/// Устанавливает значение категории для категории слов.
	/// </summary>
	/// <param name="category">Ссылка на категорию слов</param>
	void setWordCategory(WordCategory& category);

	/// <summary>
	/// Читает первое слово в строке слов и станавливает результат в строку word.
	/// </summary>
	/// <param name="word">Строка, в которую будет записан результат чтения.</param>
	void setFirstWord(std::wstring& word);

	/// <summary>
	/// Читает второе слово в строке слов и определяет его время на основании категории слова.
	/// </summary>
	/// <param name="word">Строка, в которую будет записан результат чтения второго слова.</param>
	/// <param name="category">Категория второго слова. Необходима для определения времени слова.</param>
	/// <param name="tense">Переменная, в которую будет записан результат определения времени второго слова.</param>
	void setSecondWord(std::wstring& word, const WordCategory& category, WordTense& tense);

	/// <summary>
	/// Читает и устанавливает частоту двух слов в файле.
	/// </summary>
	/// <param name="frequency">Переменная, в которую будет записан результат чтения частоты двух слов.</param>
	void setFrequency(int& frequency);

	/// <summary>
	/// Определяет является ли символ цифрой.
	/// </summary>
	/// <param name="ch">Проверяемый символ</param>
	/// <returns>Возвращает true, если символ является цифрой.</returns>
	bool isNumCh(wchar_t ch) const;

	/// <summary>
	/// Прочитывает текущую строку файла до символа переноса строки.
	/// </summary>
	void setEndOfLine();

	/// <summary>
	/// Прочитывает строку без сохранения в буффер.
	/// </summary>
	void skipLine();

};
