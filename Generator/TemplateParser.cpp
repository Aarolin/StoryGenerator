#include "TemplateParser.h"

TemplateParser::TemplateParser()
{

}

TemplateParser::TemplateParser(const std::filesystem::path p)
{
	openFile(p);
}

void TemplateParser::openFile(const std::filesystem::path p)
{
	fileReader_.open(p);
	if (!fileReader_.is_open()) {
		throw std::runtime_error("File didn't open");
	}
	std::locale loc("ru-RU.utf8");
	fileReader_.imbue(loc);
}

TemplateParser::Template TemplateParser::readFile() {

	using namespace json;
	using Template = TemplateParser::Template;
	using TokenTypeMap = TemplateParser::TokenTypeMap;

	Node root = loadNode(fileReader_);

	const Array& templatesNodes = root.asArray();
	TokenTypeMap tokenTypesMatching = getTokenTypeMap();
	Template newTemplate;

	for (const auto& templateNode : templatesNodes) {

		const Array& templateTokens = templateNode.asArray();

		for (const auto& tokenNode : templateTokens) {

			const Dict& tokenObject = tokenNode.asDict();

			TokenType tokensType = tokenTypesMatching[tokenObject.at(tokenTypeKey_).asString()];

			if (tokensType == TokenType::PLACEHOLDER) {
				newTemplate.push_back(createPlaceholderToken(tokenObject));
			}
			else if (tokensType == TokenType::TEXT) {
				newTemplate.push_back(createTextToken(tokenObject));
			}
			else {
				throw std::runtime_error("Undefined token's type!");
			}
		}
	}

	return newTemplate;
}


TemplateParser::~TemplateParser()
{

}

TemplateParser::TokenTypeMap TemplateParser::getTokenTypeMap() const
{
	TokenTypeMap tokenMap;

	tokenMap[L"PLACEHOLDER"] = TokenType::PLACEHOLDER;
	tokenMap[L"TEXT"] = TokenType::TEXT;

	return tokenMap;
}

PlaceholderToken TemplateParser::createPlaceholderToken(const json::Dict& tokenObject) const
{

	return PlaceholderToken();
}

TextToken TemplateParser::createTextToken(const json::Dict& tokenObject) const
{
	return TextToken();
}
