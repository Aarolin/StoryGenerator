#pragma once


#include "BookParser.h"
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

class BookManager {
public:
	
	std::vector<Relationship> readFile(const std::string& filename, WordDictionary& dict);
	std::vector<Relationship> readFile(const std::filesystem::path p, WordDictionary& dict);
	
	void writeFile(const std::string& filename, const std::string& data) const;

private:

	BookParser parser_;

};