#pragma once

#include "Relationship.h"
#include "Word.h"

#include <algorithm>
#include <vector>

class DataManager {
public:
	DataManager(const std::vector<Relationship>& data);
	const std::vector<RelationshipEntry>& getPerLocByKey(const Word* key) const;

private:

	RelationshipIndex perLocIndex_;
	RelationshipIndex locOrgIndex_;
	RelationshipIndex perActionIndex_;
	RelationshipIndex orgActionIndex_;

	void initializeIndexes(const std::vector<Relationship>& data);
	void sortIndexes();
	void sortIndex(RelationshipIndex& index);
};