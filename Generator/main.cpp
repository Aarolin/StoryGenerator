#pragma once

#include "BookManager.h"
#include "DataManager.h"
#include "StoryGenerator.h"
#include "TemplateManager.h"
#include "WordDictionary.h"

#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {

	using Template = TemplateParser::Template;

	WordDictionary dict;
	BookManager fileManager;
	std::string dataFileName(argv[1]);

	const std::vector<Relationship> wordsRelationships = fileManager.readFile(dataFileName, dict);
	DataManager dataManager(wordsRelationships);

	std::string templateFileName(argv[2]);
	TemplateManager tmpManager(templateFileName);

	StoryGenerator generator(dataManager, tmpManager);

	std::wstring story = generator.generateStory();

	return 0;
}