#pragma once

#include "TemplateParser.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <locale>
#include <clocale>

class TemplateManager {
public:
	using Template = TemplateParser::Template;

	TemplateManager();
	TemplateManager(const std::string& path);

	const std::vector<Template>& getTemplateList() const;

	~TemplateManager();

private:
	TemplateParser parser_;
	std::vector<Template> templateList_;
};

