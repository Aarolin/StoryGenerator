#pragma once

#include <string>
#include <unordered_map>

enum class WordCategory {
	NONE,
	PER,
	LOC,
	ORG,
	ACTION
};

enum class WordTense {
	PAST,
	PRESENT,
	FUTURE,
	NONE
};

using TenseMap = std::unordered_map<std::wstring, WordTense>;

class Word {
public:

	Word(const std::wstring& text, WordCategory category, WordTense tense = WordTense::PRESENT);

	const std::wstring& getText() const;
	WordCategory getCategory() const;
	bool operator==(const Word& other) const;

private:
	std::wstring text_;
	WordCategory category_;
	WordTense tense_;
};

// Универсальный хешер для Word и указателей на Word.
struct WordHash {
	// Для объекта типа Word
	std::size_t operator()(const Word& w) const {
		std::hash<std::wstring> hashStr;
		std::hash<int> hashInt;
		return hashStr(w.getText()) ^ (hashInt(static_cast<int>(w.getCategory())) << 1);
	}

	// Для указателя на Word
	std::size_t operator()(const Word* w) const {
		if (!w) return 0;
		// Вызываем перегруженную версию для объекта, на который указывает w
		return operator()(*w);
	}
};

struct WordPtrEqual {
	bool operator()(const Word* a, const Word* b) const {
		if (a == b) return true;
		if (!a || !b) return false;
		return a->getText() == b->getText() && a->getCategory() == b->getCategory();
	}
};

TenseMap getTenseMap();

const TenseMap WORD_TENSE_MAP = getTenseMap();