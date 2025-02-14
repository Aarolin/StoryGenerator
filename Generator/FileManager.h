#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

class FileManager {
public:
	std::unordered_map<std::string, std::vector<std::string>> readFile(const std::string& filename) const;
	void writeFile(const std::string& filename, const std::string& data) const;
};