#pragma once

#include "FileManager.h"

std::vector<Relationship> FileManager::readFile(const std::string& filename, WordDictionary& dict) const
{
	std::filesystem::path p(filename);
	return readFile(p, dict);
}

std::vector<Relationship> FileManager::readFile(const std::filesystem::path p, WordDictionary& dict) const
{
	std::locale ru("ru-RU");
	std::wifstream fileReader(p);
	fileReader.imbue(ru);

	if (!fileReader.is_open()) {
		throw("File didn't open");
	}

	WordCategory firstPairType = WordCategory::NONE;
	WordCategory secondPairType = WordCategory::NONE;

	bool firstPairTypeIsReady = false;
	bool stringAfterPairTypeSkipped = false;
	std::vector<Relationship> result;

	while (!fileReader.eof()) {
		char ch = fileReader.get();
		if (ch == '<') {

			firstPairType = getPairType(fileReader);
			goNextPairType(fileReader);
			secondPairType = getPairType(fileReader);

			// we need to go to the next line
			fileReader.get();

			skipFileLine(fileReader);

			// if the line starts with a space character then we have reached the end of the current category.
			while (fileReader.peek() != ' ') {
				// fixed sequence: read the first word -> read the second word -> read the frequency -> go to the next line
				std::wstring firstWord = readFirstWord(fileReader);
				std::wstring secondWord = readSecondWord(fileReader);
				int frequency = readFrequency(fileReader);
				auto firstWordPtr = dict.intern(firstWord, firstPairType);
				auto secondWordPtr = dict.intern(secondWord, secondPairType);
				result.push_back(Relationship(firstWordPtr, secondWordPtr, frequency));
			}

		}
	}

	return result;
}

void FileManager::writeFile(const std::string& filename, const std::string& data) const
{

}

WordCategory FileManager::getPairType(std::wifstream& fileReader) const
{
	std::wstring category;
	wchar_t ch = fileReader.get();

	while (ch != '>') {
		category += ch;
		ch = fileReader.get();
	}

	if (category == L"Персонаж") {
		return WordCategory::PER;
	}
	else if (category == L"Локация") {
		return WordCategory::LOC;
	}
	else if (category == L"Действие") {
		WordCategory::ACTION;
	}
		
	return WordCategory::ORG;
}

void FileManager::goNextPairType(std::wifstream& fileReader) const
{
	while (fileReader.peek() != '<') {
		fileReader.get();
	}
}

void FileManager::skipFileLine(std::wifstream& fileReader) const
{
	wchar_t ch = fileReader.get();

	while (ch != '\n') {
		ch = fileReader.get();
	}

	fileReader.get();
}

std::wstring FileManager::readFirstWord(std::wifstream& fileReader) const
{
	std::wstring firstWord = L"";
	wchar_t ch = fileReader.get();

	while (ch != ':') {
		firstWord += ch;
		ch = fileReader.get();
	}

	firstWord.pop_back();
	return firstWord;
}

std::wstring FileManager::readSecondWord(std::wifstream& fileReader) const
{
	skipSpacesInLine(fileReader);

	std::wstring secondWord = L"";
	wchar_t ch = fileReader.peek();

	while (!isNumSymbol(ch)) {
		fileReader.get();
		secondWord += ch;
		ch = fileReader.peek();
	}

	secondWord.pop_back();
	return secondWord;
}

int FileManager::readFrequency(std::wifstream& fileReader) const
{
	skipSpacesInLine(fileReader);

	std::wstring frequency = L"";
	wchar_t ch = fileReader.get();

	while (ch != ' ' and ch != '\n') {
		frequency += ch;
	}

	return std::stoi(frequency);
}

void FileManager::skipSpacesInLine(std::wifstream& fileReader) const
{
	wchar_t ch = fileReader.peek();
	while (ch == ' ' or ch == ':') {
		fileReader.get();
		ch = fileReader.peek();
	}

}

bool FileManager::isNumSymbol(wchar_t ch) const
{
	return ch >= 48 && ch <= 57;
}
