#include "FileParser.h"

FileParser::FileParser()
{

}

void FileParser::setFilePath(const std::filesystem::path p)
{
	fileReader_.open(p);
	if (!fileReader_.is_open()) {
		throw std::runtime_error("File didn't open");
	}
	std::locale loc("ru-RU.utf8");
	fileReader_.imbue(loc);
}

bool FileParser::eof() const
{
	return fileReader_.eof();
}

void FileParser::readCategoryLine()
{
	readNextLine();
}

void FileParser::readNextLine()
{
	std::wstring line = L"";
	std::getline(fileReader_, line);
	buffer_.str(line);

}

void FileParser::skipLine()
{
	std::wstring line = L"";
	std::getline(fileReader_, line);

}

WordCategory FileParser::getWordCategory(bool goNextLine)
{
	
	char ch = buffer_.get();
	while (ch != '<') {
		char ch = buffer_.get();
	}

	std::wstring strCategory = L"";

	while (ch != '>') {
		ch = buffer_.get();
		strCategory += ch;
	}

	WordCategory wCategory = WordCategory::NONE;

	if (strCategory == L"Персонаж") {
		wCategory =  WordCategory::PER;
	}
	else if (strCategory == L"Локация") {
		wCategory =  WordCategory::LOC;
	}
	else if (strCategory == L"Действие") {
		wCategory = WordCategory::ACTION;
	}
	else if (strCategory == L"Организация") {
		wCategory = WordCategory::ORG;
	}

	if (goNextLine) {
		buffer_.clear();
	}

	return wCategory;
}

std::wstring FileParser::getFirstWord()
{
	return std::wstring();
}

std::wstring FileParser::getSecondWord()
{
	return std::wstring();
}

int FileParser::getFrequency()
{
	return 0;
}
