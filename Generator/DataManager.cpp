#include "DataManager.h"

DataManager::DataManager(const std::vector<Relationship>& data)
{
	initializeIndexes(data);
	sortIndexes();
}

const std::vector<RelationshipEntry>& DataManager::getPerLocByKey(const Word* key) const
{
	return perLocIndex_.getEntries(key);
}

void DataManager::initializeIndexes(const std::vector<Relationship>& data)
{
	for (const auto& relation : data) {

		const Word* leftWordPtr = relation.getLeft();
		const Word* rightWordPtr = relation.getRight();
		int frequency = relation.getFrequency();
		WordCategory leftWordCategory = leftWordPtr->getCategory();
		WordCategory rightWordCategory = rightWordPtr->getCategory();
		RelationshipIndex* index = &orgActionIndex_;

		if (leftWordCategory == WordCategory::PER and rightWordCategory == WordCategory::LOC) {
			index = &perLocIndex_;
		}
		else if (leftWordCategory == WordCategory::LOC and rightWordCategory == WordCategory::ORG) {
			index = &locOrgIndex_;
		}
		else if (leftWordCategory == WordCategory::PER and rightWordCategory == WordCategory::ACTION) {
			index = &perActionIndex_;
		}

		index->addEntry(leftWordPtr, rightWordPtr, frequency);
		index->addEntry(rightWordPtr, leftWordPtr, frequency);
	}
}

void DataManager::sortIndexes()
{
	sortIndex(perLocIndex_);
	sortIndex(locOrgIndex_);
	sortIndex(perActionIndex_);
	sortIndex(orgActionIndex_);
}

void DataManager::sortIndex(RelationshipIndex& index)
{
	for (auto& [word, entries] : index) {
		std::sort(entries.begin(), entries.end(), [](const RelationshipEntry& lhs, const RelationshipEntry& rhs) {
			return lhs.getFrequency() > rhs.getFrequency();
			});
	}
}
