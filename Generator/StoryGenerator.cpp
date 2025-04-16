#include "StoryGenerator.h"

StoryGenerator::StoryGenerator(const DataManager& dataManager, const TemplateManager& templateManager) : dataManager_(dataManager), templateManager_(templateManager)
{

}

std::wstring StoryGenerator::generateStory() const
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
	std::random_device rd;
	std::mt19937 gen(rd()); 

	size_t min = 0;
	size_t max = templates.size() - 1;

	std::uniform_int_distribution<> distrib(min, max);

	size_t randomTemplateId = distrib(gen);

	return templates[randomTemplateId];
}

std::wstring StoryGenerator::generateStoryFromTemplate(const Template& randomTemplate) const
{
	std::wstring story = L"";

	for (const TemplateToken& token : randomTemplate) {

		if (std::holds_alternative<PlaceholderToken>(token)) {

			const PlaceholderToken& pcToken = std::get<PlaceholderToken>(token);

			if (!placeholderInCache(pcToken)) {
				addPlaceholderToCache(pcToken);
				std::wstring placeholderTextValue = evaluatePlaceholder(pcToken);
				cachePlaceholderTokenValue(pcToken, placeholderTextValue);
			}

			const std::wstring& placeholderTextValue = getPlaceholderTextValueFromCache(pcToken);
			includeTextToStory(placeholderTextValue, story);
			
		}
		else if (std::holds_alternative<TextToken>(token)) {

			const TextToken& txtToken = std::get<TextToken>(token);
			includeTextTokenToStory(txtToken, story);

		}
	}

	return story;
}

void StoryGenerator::includeTextTokenToStory(const TextToken& txtToken, std::wstring& story) const 
{
	const std::wstring& text = txtToken.getText();
	includeTextToStory(text, story);
}

void StoryGenerator::includeTextToStory(const std::wstring& text, std::wstring& story) const
{
	story += text;
}

bool StoryGenerator::placeholderInCache(const PlaceholderToken& token) const
{
	return false;
}

void StoryGenerator::addPlaceholderToCache(const PlaceholderToken& token) const
{

}

std::wstring StoryGenerator::evaluatePlaceholder(const PlaceholderToken& token) const
{
	return std::wstring();
}

void StoryGenerator::cachePlaceholderTokenValue(const PlaceholderToken& token, const std::wstring& text) const
{

}

const std::wstring& StoryGenerator::getPlaceholderTextValueFromCache(const PlaceholderToken& token) const
{
	// TODO: вставьте здесь оператор return
}
