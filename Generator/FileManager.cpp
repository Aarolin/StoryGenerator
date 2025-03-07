#pragma once

#include "FileManager.h"

std::vector<Relationship> FileManager::readFile(const std::string& filename, WordDictionary& dict)
{
	std::filesystem::path p(filename);
	return readFile(p, dict);
}

std::vector<Relationship> FileManager::readFile(const std::filesystem::path p, WordDictionary& dict)
{
	parser_.setFilePath(p);

	std::vector<Relationship> result;

	while (!parser_.eof()) {

		//if (ch == '<') {

		//	firstPairType = getPairType(fileReader);
		//	goNextPairType(fileReader);
		//	secondPairType = getPairType(fileReader);
		//	fileReader.get();

		//	skipFileLine(fileReader);

		//	// Если строка начинается с символа переноса строки, тогда перед нами окончание текущей пары категорий.
		//	while (fileReader.peek() != '\n') {

		//		// Фиксированная последовательность действий: считываем первое слово -> затем второе -> время ко второму, если оно есть -> частоту к паре слов.
		//		std::wstring firstWord = readFirstWord(fileReader);
		//		if (firstWord.empty()) {
		//			break;
		//		}
		//		std::wstring secondWord = readSecondWord(fileReader);
		//		WordTense secondWordTense = WordTense::PRESENT;

		//		if (secondPairType == WordCategory::ACTION) {
		//			secondWordTense = readWordTense(fileReader);
		//		}

		//		int frequency = readFrequency(fileReader);

		//		// На месте первой категории всегда стоят слова существительные, которые по умолчанию 
		//		// являются словами в настоящем времени.
		//		auto firstWordPtr = dict.intern(firstWord, firstPairType);

		//		// Глаголы всегда будут стоять на месте второй категории в паре категорий, поэтому явное считывание 
		//		// времени слова может пригодиться только тут.
		//		auto secondWordPtr = dict.intern(secondWord, secondPairType, secondWordTense);

		//		result.push_back(Relationship(firstWordPtr, secondWordPtr, frequency));
		//	}

		//}
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
		return WordCategory::ACTION;
	}
		
	return WordCategory::ORG;
}

void FileManager::goNextPairType(std::wifstream& fileReader) const
{
	while (fileReader.peek() != '<') {
		fileReader.get();
	}
	fileReader.get();
}

void FileManager::skipFileLine(std::wifstream& fileReader) const
{
	wchar_t ch = fileReader.get();

	while (ch != '\n') {
		ch = fileReader.get();
	}

}

std::wstring FileManager::readFirstWord(std::wifstream& fileReader) const
{
	std::wstring firstWord = L"";
	wchar_t ch = fileReader.get();

	while (ch != ':' && !fileReader.eof()) {
		firstWord += ch;
		ch = fileReader.get();
	}
	if (!firstWord.empty()) {
		firstWord.pop_back();
	}
	return firstWord;
}

std::wstring FileManager::readSecondWord(std::wifstream& fileReader) const
{
	skipExtraCharsInLine(fileReader);

	std::wstring secondWord = L"";
	wchar_t ch = fileReader.get();

	while (ch != '\t') {
		secondWord += ch;
		ch = fileReader.get();
		if (ch == '(') {
			secondWord.pop_back();
			break;
		}
	}

	return secondWord;
}

int FileManager::readFrequency(std::wifstream& fileReader) const
{
	skipExtraCharsInLine(fileReader);

	std::wstring frequency = L"";
	wchar_t ch = fileReader.get();

	while (ch != ' ' and ch != '\n') {
		frequency += ch;
		ch = fileReader.get();
	}

	return std::stoi(frequency);
}

WordTense FileManager::readWordTense(std::wifstream& fileReader) const
{
	skipExtraCharsInLine(fileReader);
	std::wstring tense = L"";
	wchar_t ch = fileReader.get();

	while (ch != ')') {
		tense += ch;
		ch = fileReader.get();
	}

	if (tense == L"прошедшее") {
		return WordTense::PAST;
	}
	else if (tense == L"будущее") {
		return WordTense::FUTURE;
	}

	return WordTense::PRESENT;
}

void FileManager::skipExtraCharsInLine(std::wifstream& fileReader) const
{
	wchar_t ch = fileReader.peek();
	while (ch == ' ' or ch == ':' or ch == '\t') {
		fileReader.get();
		ch = fileReader.peek();
	}
}

