#pragma once

#include "json.h"
#include "TemplateToken.h"

#include <utility>

using TokenTypeMap = std::unordered_map<std::wstring, TokenType>;
using TokenKeysMap = std::unordered_map<std::wstring, std::wstring>;
using PlaceholderTypeMap = std::unordered_map<std::wstring, PlaceholderType>;
using TemplateToken = std::variant<PlaceholderToken, TextToken>;

class TemplateParser {

public:
	using Template = std::vector<TemplateToken>;

	TemplateParser();
	TemplateParser(const std::filesystem::path p);

	void openFile(const std::filesystem::path p);

	std::vector<Template> readFile();

	~TemplateParser();

private:

	std::wifstream fileReader_;
	
	PlaceholderToken createPlaceholderToken(const json::Dict& tokenObject) const;
	TextToken createTextToken(const json::Dict& tokenObject) const;

};

TokenTypeMap getTokenTypeMap();
TokenKeysMap getTokenKeysMap();
PlaceholderTypeMap getPlaceholderTypeMap();


const TokenTypeMap TOKEN_TYPE_MAP = getTokenTypeMap();
const TokenKeysMap TOKEN_KEYS_MAP = getTokenKeysMap();
const PlaceholderTypeMap PLACEHOLDER_TYPE_MAP = getPlaceholderTypeMap();