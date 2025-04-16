#pragma once

#include <random>

#include "DataManager.h"
#include "TemplateManager.h"


class StoryGenerator {
public:
	using Template = TemplateParser::Template;

	StoryGenerator(const DataManager& dataManager, const TemplateManager& templateManager);
	std::wstring generateStory() const;

	~StoryGenerator();

private:

	const DataManager& dataManager_;
	const TemplateManager& templateManager_;

	const Template& chooseRandomTemplate(const std::vector<Template>& templates) const;
	std::wstring generateStoryFromTemplate(const Template& randomTemplate) const;

	void includeTextTokenToStory(const TextToken& txtToken, std::wstring& story) const;
	void includeTextToStory(const std::wstring& text, std::wstring& story) const;

	bool placeholderInCache(const PlaceholderToken& token) const;
	void addPlaceholderToCache(const PlaceholderToken& token) const;
	std::wstring evaluatePlaceholder(const PlaceholderToken& token) const;

	void cachePlaceholderTokenValue(const PlaceholderToken& token, const std::wstring& text) const;
	const std::wstring& getPlaceholderTextValueFromCache(const PlaceholderToken& token) const;

};

using TemplateToken = std::variant<PlaceholderToken, TextToken>;