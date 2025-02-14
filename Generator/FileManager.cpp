#pragma once

#include "FileManager.h"


std::unordered_map<std::string, std::vector<std::string>> FileManager::readFile(const std::string& filename) const
{
	std::unordered_map<std::string, std::vector<std::string>> result;

	std::ifstream fileReader(filename);

	if (!fileReader.is_open()) 
	{
		throw("File didn't open");
	}

	std::string fileData = "";

	while (!fileReader.eof()) 
	{

	}

	return result;
}

void FileManager::writeFile(const std::string& filename, const std::string& data) const
{

}
