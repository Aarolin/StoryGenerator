#pragma once

#include "FileManager.h"
#include "WordDictionary.h"

#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {

	std::setlocale(LC_ALL, "ru_RU.utf8");
	std::locale::global(std::locale("ru_RU.utf8"));

	WordDictionary dict;
	FileManager fileManager;
	std::string filename(argv[1]);
	auto wordsRelationships = fileManager.readFile(filename, dict);

	return 0;
}