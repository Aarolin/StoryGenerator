#include "WordDictionary.h"

WordDictionary::WordDictionary()
{
}

const Word* WordDictionary::intern(const std::wstring& text, WordCategory category, WordTense tense)
{
	Word temp(text, category, tense);

	auto it = words_.find(temp);

	if (it == words_.end()) {
		auto insertOperation = words_.insert(temp);
		return &(*insertOperation.first);
	}

	return &(*it);
}
