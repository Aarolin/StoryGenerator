#pragma once
#include "DataManager.h"
#include "TemplateManager.h"


class StoryGenerator {
public:
	StoryGenerator(const DataManager& dataManager, const TemplateManager& templateManager);
	~StoryGenerator();

private:

	const DataManager& dataManager_;
	const TemplateManager& templateManager_;

};

