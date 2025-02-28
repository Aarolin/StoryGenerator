#pragma once
#include<string>

enum class WordCategory {
	NONE,
	PER,
	LOC,
	ORG,
	ACTION
};

class Word {
public:

	Word(const std::wstring& text, WordCategory category);

	const std::wstring& getText() const;
	WordCategory getCategory() const;
	bool operator==(const Word& other) const;

private:
	std::wstring text_;
	WordCategory category_;
};

// ������������� ����� ��� Word � ���������� �� Word.
struct WordHash {
	// ��� ������� ���� Word
	std::size_t operator()(const Word& w) const {
		std::hash<std::wstring> hashStr;
		std::hash<int> hashInt;
		return hashStr(w.getText()) ^ (hashInt(static_cast<int>(w.getCategory())) << 1);
	}

	// ��� ��������� �� Word
	std::size_t operator()(const Word* w) const {
		if (!w) return 0;
		// �������� ������������� ������ ��� �������, �� ������� ��������� w
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