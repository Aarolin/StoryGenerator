#pragma once

#include "BookManager.h"
#include "WordDictionary.h"
#include "DataManager.h"
#include "TemplateParser.h"

#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {


	WordDictionary dict;
	BookManager fileManager;
	std::string dataFileName(argv[1]);

	auto wordsRelationships = fileManager.readFile(dataFileName, dict);
	DataManager dataManager(wordsRelationships);

	std::string templateFileName(argv[2]);
	std::filesystem::path tmpPath(templateFileName);
	TemplateParser tmpParser(tmpPath);
	tmpParser.readFile();

	return 0;
}