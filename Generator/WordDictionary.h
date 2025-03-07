#pragma once

#include "Word.h"
#include <unordered_set>


class WordDictionary {
public:
	WordDictionary();
	const Word* intern(const std::wstring& text, WordCategory category, WordTense tense = WordTense::PRESENT);

private:
	std::unordered_set<Word, WordHash> words_;
};
