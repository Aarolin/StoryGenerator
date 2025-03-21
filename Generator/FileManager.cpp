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

		auto categoryLine = parser_.readCategoryLine();
		WordCategory leftWordsCategory = categoryLine.leftWordsCategory;
		WordCategory rightWordsCategory = categoryLine.rightWordsCategory;

		while (!parser_.isEndOfCategory()) {
			try {
				auto parsedRecord = parser_.readWordsLine(leftWordsCategory, rightWordsCategory);
				if (!parsedRecord.success) {
					break;
				}

				auto firstWordPtr = dict.intern(parsedRecord.firstWord, leftWordsCategory);
				auto secondWordPtr = dict.intern(parsedRecord.secondWord, rightWordsCategory, parsedRecord.secondWordTense);

				result.push_back(Relationship(firstWordPtr, secondWordPtr, parsedRecord.frequency));
			}
			catch (std::out_of_range& rangeError) {
				std::cerr << rangeError.what();
				continue;
			}
		}

		parser_.setNextCategory();
	}

	return result;
}

void FileManager::writeFile(const std::string& filename, const std::string& data) const
{

}


