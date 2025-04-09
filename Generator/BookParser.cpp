#include "BookParser.h"

BookParser::BookParser()
{

}

void BookParser::openFile(const std::filesystem::path p)
{
	fileReader_.open(p);
	if (!fileReader_.is_open()) {
		throw std::runtime_error("File didn't open");
	}
	std::locale loc("ru-RU.utf8");
	fileReader_.imbue(loc);
}

bool BookParser::eof() const
{
	return fileReader_.eof();
}

bool BookParser::isEndOfCategory() const
{
	return endOfCategory_;
}

BookParser::ParsedCategoryRecord BookParser::readCategoryLine()
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

	BookParser::ParsedCategoryRecord categoryRecord;

	setWordCategory(categoryRecord.leftWordsCategory);
	setWordCategory(categoryRecord.rightWordsCategory);
	skipLine();
	isWordsArea_ = true;
	return categoryRecord;
}

BookParser::ParsedRecord BookParser::readWordsLine(const WordCategory& leftWordsCategory, const WordCategory& rightWordsCategory)
{
	if (!isWordsArea_) {
		throw std::out_of_range("Error file reading. String does not contain words");
	}

	BookParser::ParsedRecord record;
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

void BookParser::setNextCategory()
{
	endOfCategory_ = false;
}

void BookParser::setEndOfLine()
{
	wchar_t ch = fileReader_.get();

	while (ch != '\n') {
		ch = fileReader_.get();
	}

	buffer_.clear();
}

void BookParser::skipLine()
{
	std::wstring line = L"";
	std::getline(fileReader_, line);
}

bool BookParser::readNextLine()
{
	buffer_.clear();
	
	std::wstring line = L"";
	std::getline(fileReader_, line);
	buffer_.str(line);

	return !line.empty();
}

void BookParser::setWordCategory(WordCategory& category)
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

void BookParser::setFirstWord(std::wstring& word)
{
	std::getline(buffer_, word, wordSeparator_);

	if (!word.empty()) {
		word.pop_back();
	}
	
}

void BookParser::setSecondWord(std::wstring& word, const WordCategory& category, WordTense& tense)
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

void BookParser::setFrequency(int& frequency)
{
	std::wstring num = L"";
	std::getline(buffer_, num);

	frequency = std::stoi(num);
}

bool BookParser::isNumCh(wchar_t ch) const
{
	return std::iswdigit(ch);
}
