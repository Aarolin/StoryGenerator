#include "TemplateManager.h"

TemplateManager::TemplateManager()
{
}

TemplateManager::TemplateManager(const std::string& path) 
{
	std::filesystem::path p(path);
	parser_.openFile(p);
	templateList_ = parser_.readFile();
}

const std::vector<TemplateParser::Template>& TemplateManager::getTemplateList() const
{
	return templateList_;
}

TemplateManager::~TemplateManager()
{

}
