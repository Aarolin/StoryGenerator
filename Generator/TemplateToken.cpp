#include "TemplateToken.h"

TemplateToken::TemplateToken(TokenType type) : type_(type)
{
}

TokenType TemplateToken::getType() const
{
    return type_;
}

PlaceholderToken::PlaceholderToken() : TemplateToken(TokenType::PLACEHOLDER), placeholderType_(PlaceholderType::NONE), tense_(WordTense::NONE)
{
}

PlaceholderToken::PlaceholderToken(size_t indexUsage, PlaceholderType placeholderType, WordTense tense) : 
    TemplateToken(TokenType::PLACEHOLDER), indexUsage_(indexUsage), placeholderType_(placeholderType), tense_(tense)
{
}

PlaceholderType PlaceholderToken::getPlaceholderType() const
{
    return placeholderType_;
}

size_t PlaceholderToken::getIndex() const
{
    return indexUsage_;
}

WordTense PlaceholderToken::getTense() const
{
    return WordTense();
}

TextToken::TextToken() : TemplateToken(TokenType::TEXT)
{

}

TextToken::TextToken(std::wstring text) : TemplateToken(TokenType::TEXT), value_(std::move(text))
{
}

const std::wstring& TextToken::getText() const
{
    return value_;
}
