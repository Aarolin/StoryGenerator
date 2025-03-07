#pragma once

#include "FileManager.h"
#include "WordDictionary.h"

#include <cstdlib>
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {


	WordDictionary dict;
	FileManager fileManager;
	std::string filename(argv[1]);
	auto wordsRelationships = fileManager.readFile(filename, dict);

	return 0;
}