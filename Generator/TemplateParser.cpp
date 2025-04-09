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

void TemplateParser::readFile() {

	using namespace json;
	Node root = loadNode(fileReader_);

}


TemplateParser::~TemplateParser()
{

}
