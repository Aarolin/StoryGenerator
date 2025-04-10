#pragma once

#include "Word.h"

enum class TokenType {
	PLACEHOLDER,
	TEXT,
	NONE
};

enum class PlaceholderType {
	HERO_NAME,
	RANDOM_INTEGER,
	LOCATION_NAME,
	ORGANIZATION,
	ACTION,
	NONE
};

class TemplateToken {
public:
	TemplateToken(TokenType type);

	TokenType getType() const;

private:
	TokenType type_;
};

class PlaceholderToken : public TemplateToken {
public:
	PlaceholderToken();
	PlaceholderToken(size_t indexUsage, PlaceholderType placeholderType, WordTense tense);

	PlaceholderType getPlaceholderType() const;
	size_t getIndex() const;
	WordTense getTense() const;

private:
	size_t indexUsage_ = 0;
	PlaceholderType placeholderType_;
	WordTense tense_;

};

class TextToken : public TemplateToken {
public:
	TextToken();
	TextToken(std::wstring text);
	const std::wstring& getText() const;
	
private:
	std::wstring value_;
};