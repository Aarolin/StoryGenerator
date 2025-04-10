#pragma once

#include "json.h"
#include "TemplateToken.h"

class TemplateParser {

public:
	using Template = std::vector<TemplateToken>;
	using TokenTypeMap = std::unordered_map<std::wstring, TokenType>;

	TemplateParser();
	TemplateParser(const std::filesystem::path p);

	void openFile(const std::filesystem::path p);

	Template readFile();

	~TemplateParser();

private:

	std::wifstream fileReader_;
	const std::wstring tokenTypeKey_ = L"token_type";

	TokenTypeMap getTokenTypeMap() const;
	PlaceholderToken createPlaceholderToken(const json::Dict& tokenObject) const;
	TextToken createTextToken(const json::Dict& tokenObject) const;

};
