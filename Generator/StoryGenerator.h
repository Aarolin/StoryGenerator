#pragma once

#include <random>

#include "DataManager.h"
#include "TemplateManager.h"


class StoryGenerator {
public:
	using Template = TemplateParser::Template;

	StoryGenerator(const DataManager& dataManager, const TemplateManager& templateManager);
	std::wstring generateStory();

	~StoryGenerator();

private:

	struct TokenVisitor {

		StoryGenerator& self;

		std::wstring operator()(const TextToken& txtToken) {
			return txtToken.getText();
		}

		std::wstring operator()(const PlaceholderToken& pcToken) {

			return self.evaluatePlaceholder(pcToken);
		}

	};

	const DataManager& dataManager_;
	const TemplateManager& templateManager_;

	std::random_device rd_;
	std::mt19937 engine_;

	const Template& chooseRandomTemplate(const std::vector<Template>& templates) const;
	std::wstring generateStoryFromTemplate(const Template& randomTemplate);

	std::wstring evaluatePlaceholder(const PlaceholderToken& token) const;

};

using TemplateToken = std::variant<PlaceholderToken, TextToken>;

