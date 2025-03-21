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

bool FileParser::isEndOfCategory() const
{
	return endOfCategory_;
}

FileParser::ParsedCategoryRecord FileParser::readCategoryLine()
{
	bool isCategoryLine = false;

	while (!isCategoryLine) {
		readNextLine();
		const std::wstring& lastLine = buffer_.str();
		auto it = find(lastLine.begin(), lastLine.end(), '<');
		if (it != lastLine.end()) {
			isCategoryLine = true;
		}
	}

	FileParser::ParsedCategoryRecord categoryRecord;

	setWordCategory(categoryRecord.leftWordsCategory);
	setWordCategory(categoryRecord.rightWordsCategory);
	skipLine();
	isWordsArea_ = true;
	return categoryRecord;
}

FileParser::ParsedRecord FileParser::readWordsLine(const WordCategory& leftWordsCategory, const WordCategory& rightWordsCategory)
{
	if (!isWordsArea_) {
		throw std::out_of_range("Error file reading. String does not contain words");
	}

	FileParser::ParsedRecord record;
	bool successfulReading = readNextLine();

	if (!successfulReading) {
		endOfCategory_ = true;
		return record;
	}
	
	setFirstWord(record.firstWord);
	setSecondWord(record.secondWord, rightWordsCategory, record.secondWordTense);
	setFrequency(record.frequency);
	record.success = true;

	return record;
}

void FileParser::setNextCategory()
{
	endOfCategory_ = false;
}

void FileParser::setEndOfLine()
{
	char ch = fileReader_.get();

	while (ch != '\n') {
		ch = fileReader_.get();
	}

	buffer_.clear();
}

void FileParser::skipLine()
{
	std::wstring line = L"";
	std::getline(fileReader_, line);
}

bool FileParser::readNextLine()
{
	buffer_.clear();
	
	std::wstring line = L"";
	std::getline(fileReader_, line);
	buffer_.str(line);

	return !line.empty();
}

void FileParser::setWordCategory(WordCategory& category)
{
	std::wstring extraCharacters = L"";
	
	std::getline(buffer_, extraCharacters, wordCategoryStartCh_);

	std::wstring strCategory = L"";
	std::getline(buffer_, strCategory, wordCategoryEndCh_);

	WordCategory wCategory = WordCategory::NONE;

	if (strCategory == L"Персонаж") {
		wCategory = WordCategory::PER;
	}
	else if (strCategory == L"Локация") {
		wCategory = WordCategory::LOC;
	}
	else if (strCategory == L"Действие") {
		wCategory = WordCategory::ACTION;
	}
	else if (strCategory == L"Организация") {
		wCategory = WordCategory::ORG;
	}

	category = wCategory;
}

void FileParser::setFirstWord(std::wstring& word)
{
	std::getline(buffer_, word, wordSeparator_);

	if (!word.empty()) {
		word.pop_back();
	}
	
}

void FileParser::setSecondWord(std::wstring& word, const WordCategory& category, WordTense& tense)
{
	wchar_t ch = buffer_.peek();
	while (ch == ' ') {
		buffer_.get();
		ch = buffer_.peek();
	}

	wchar_t terminateCh = '\t';
	bool readWordTense = false;

	if (category == WordCategory::ACTION) {
		terminateCh = '(';
		readWordTense = true;
	}

	std::getline(buffer_, word, terminateCh);

	if (readWordTense) {
		if (!word.empty()) {
			word.pop_back();
		}
		std::wstring wordTense = L"";
		wchar_t wordTenseTerminateCh = ')';
		std::getline(buffer_, wordTense, wordTenseTerminateCh);

		if (wordTense == L"прошедшее") {
			tense = WordTense::PAST;
		}
		else if (wordTense == L"будущее") {
			tense = WordTense::FUTURE;
		}
	}

}

void FileParser::setFrequency(int& frequency)
{
	std::wstring num = L"";
	std::getline(buffer_, num);

	frequency = std::stoi(num);
}

bool FileParser::isNumCh(wchar_t ch) const
{
	return std::iswdigit(ch);
}
