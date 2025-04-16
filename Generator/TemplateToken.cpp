#include "TemplateToken.h"

PlaceholderToken::PlaceholderToken() : placeholderType_(PlaceholderType::NONE), tense_(WordTense::NONE)
{
}

PlaceholderToken::PlaceholderToken(size_t indexUsage, PlaceholderType placeholderType, WordTense tense) : indexUsage_(indexUsage), 
                                                                                                          placeholderType_(placeholderType), tense_(tense)
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
    return tense_;
}

TextToken::TextToken()
{

}

TextToken::TextToken(std::wstring text) : value_(std::move(text))
{
}

const std::wstring& TextToken::getText() const
{
    return value_;
}
