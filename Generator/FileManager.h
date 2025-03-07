#pragma once


#include "FileParser.h"
#include "Relationship.h"
#include "WordDictionary.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <locale>
#include <clocale>

class FileManager {
public:
	
	std::vector<Relationship> readFile(const std::string& filename, WordDictionary& dict);
	std::vector<Relationship> readFile(const std::filesystem::path p, WordDictionary& dict);
	
	void writeFile(const std::string& filename, const std::string& data) const;

private:

	WordCategory getPairType(std::wifstream& fileReader) const;
	void goNextPairType(std::wifstream& fileReader) const;

	void skipFileLine(std::wifstream& fileReader) const;

	std::wstring readFirstWord(std::wifstream& fileReader) const;
	std::wstring readSecondWord(std::wifstream& fileReader) const;
	int readFrequency(std::wifstream& fileReader) const;

	WordTense readWordTense(std::wifstream& fileReader) const;

	// Ћишние символы - пробелы и табул€ци€
	void skipExtraCharsInLine(std::wifstream& fileReader) const;

	FileParser parser_;

};