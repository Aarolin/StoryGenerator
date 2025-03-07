#include "Word.h"

Word::Word(const std::wstring& text, WordCategory category, WordTense tense) : text_(text), category_(category), tense_(tense)
{
}

const std::wstring& Word::getText() const
{
	return text_;
}

WordCategory Word::getCategory() const
{
	return category_;
}

bool Word::operator==(const Word& other) const
{
	return text_ == other.text_ && category_ == other.category_;
}





