#include "WordDictionary.h"

WordDictionary::WordDictionary()
{
}

const Word* WordDictionary::intern(const std::wstring& text, WordCategory category)
{
	Word temp(text, category);

	auto it = words_.find(temp);

	if (it == words_.end()) {
		words_.insert(temp);
	}

	return &(*it);
}
