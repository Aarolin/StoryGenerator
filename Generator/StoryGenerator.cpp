#include "StoryGenerator.h"

StoryGenerator::StoryGenerator(const DataManager& dataManager, const TemplateManager& templateManager) : 
	dataManager_(dataManager), templateManager_(templateManager), rd_(), engine_(rd_())
{

}

std::wstring StoryGenerator::generateStory()
{
	using Template = TemplateParser::Template;
	
	const std::vector<Template> templates = templateManager_.getTemplateList();

	if (templates.empty()) {
		return L"";
	}

	const Template& randomTemplate = chooseRandomTemplate(templates);
	std::wstring story = generateStoryFromTemplate(randomTemplate);

	return story;
}



StoryGenerator::~StoryGenerator()
{

}

const StoryGenerator::Template& StoryGenerator::chooseRandomTemplate(const std::vector<Template>& templates) const
{

	size_t min = 0;
	size_t max = templates.size() - 1;

	std::uniform_int_distribution<> distrib(min, max);

	size_t randomTemplateId = distrib(engine_);

	return templates[randomTemplateId];
}

std::wstring StoryGenerator::generateStoryFromTemplate(const Template& randomTemplate)
{
	std::wstring story = L"";

	for (const TemplateToken& token : randomTemplate) {
		story += std::visit(TokenVisitor{*this}, token);
	}

	return story;
}


std::wstring StoryGenerator::evaluatePlaceholder(const PlaceholderToken& token) const
{


	return std::wstring();
}



