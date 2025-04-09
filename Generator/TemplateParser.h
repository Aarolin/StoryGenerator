#pragma once

#include "json.h"


class TemplateParser {

public:
	TemplateParser();
	TemplateParser(const std::filesystem::path p);

	void openFile(const std::filesystem::path p);
	void readFile();

	~TemplateParser();

private:

	std::wifstream fileReader_;

};
