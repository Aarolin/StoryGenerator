#pragma once

#include "Word.h"
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

class FileParser {
public:

	FileParser();
	
	void setFilePath(const std::filesystem::path p);

	bool eof() const;

	void readCategoryLine();
	void readNextLine();
	void skipLine();

	WordCategory getWordCategory(bool goNextLine = false);

	std::wstring getFirstWord();
	std::wstring getSecondWord();
	int getFrequency();

private:
	std::wistringstream buffer_;
	std::wifstream fileReader_;
};
