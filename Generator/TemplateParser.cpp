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

std::vector<TemplateParser::Template> TemplateParser::readFile() {

	using namespace json;
	
	Node root = loadNode(fileReader_);

	const Array& templatesNodes = root.asArray();
	std::vector<Template> templates;

	for (const auto& templateNode : templatesNodes) {

		Template newTemplate;
		const Array& templateTokens = templateNode.asArray();

		for (const auto& tokenNode : templateTokens) {

			const Dict& tokenObject = tokenNode.asDict();

			const auto& tokenTypeKey = TOKEN_KEYS_MAP.at(L"tokenType");
			const auto& txtTokenType = tokenObject.at(tokenTypeKey).asString();

			TokenType tokensType = TOKEN_TYPE_MAP.at(txtTokenType);

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
		templates.push_back(std::move(newTemplate));
	}

	return templates;
}


TemplateParser::~TemplateParser()
{

}

PlaceholderToken TemplateParser::createPlaceholderToken(const json::Dict& tokenObject) const
{
	size_t indexUsage = tokenObject.at(TOKEN_KEYS_MAP.at(L"indexUsage")).asInt();

	const std::wstring& txtPlaceHolderType = tokenObject.at(TOKEN_KEYS_MAP.at(L"placeholderType")).asString();
	PlaceholderType plcType = PLACEHOLDER_TYPE_MAP.at(txtPlaceHolderType);

	WordTense tense = WordTense::PRESENT;
	if (tokenObject.count(TOKEN_KEYS_MAP.at(L"tense")) == 1) {
		const std::wstring& txtTense = tokenObject.at(TOKEN_KEYS_MAP.at(L"tense")).asString();
		tense = WORD_TENSE_MAP.at(txtTense);
	}

	return PlaceholderToken(indexUsage, plcType, tense);
}

TextToken TemplateParser::createTextToken(const json::Dict& tokenObject) const
{

	const std::wstring& text = tokenObject.at(TOKEN_KEYS_MAP.at(L"textValue")).asString();

	return TextToken(text);
}

TokenTypeMap getTokenTypeMap()
{
	TokenTypeMap tokenMap;

	tokenMap[L"PLACEHOLDER"] = TokenType::PLACEHOLDER;
	tokenMap[L"TEXT"] = TokenType::TEXT;

	return tokenMap;
}

TokenKeysMap getTokenKeysMap()
{
	TokenKeysMap keysMap;

	keysMap[L"tokenType"] = L"token_type";
	keysMap[L"placeholderType"] = L"placeholder_type";
	keysMap[L"indexUsage"] = L"index_usage";
	keysMap[L"tense"] = L"tense";
	keysMap[L"textValue"] = L"value";

	return keysMap;
}

PlaceholderTypeMap getPlaceholderTypeMap()
{
	PlaceholderTypeMap placeholderTypeMap;

	placeholderTypeMap[L"HERO_NAME"] = PlaceholderType::HERO_NAME;
	placeholderTypeMap[L"ACTION"] = PlaceholderType::ACTION;
	placeholderTypeMap[L"LOCATION_NAME"] = PlaceholderType::LOCATION_NAME;
	placeholderTypeMap[L"ORGANIZATION"] = PlaceholderType::ORGANIZATION;
	placeholderTypeMap[L"RANDOM_INTEGER"] = PlaceholderType::RANDOM_INTEGER;

	return placeholderTypeMap;
}
