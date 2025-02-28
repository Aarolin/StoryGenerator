#include "Relationship.h"

RelationshipEntry::RelationshipEntry(const Word* partner, int frequency) : partner_(partner), frequency_(frequency)
{
}

const Word* RelationshipEntry::getPartner() const
{
	return partner_;
}

int RelationshipEntry::getFrequency() const
{
	return frequency_;
}

RelationshipIndex::RelationshipIndex()
{
}

void RelationshipIndex::addEntry(const Word* key, const Word* partner, int frequency)
{
	auto& partners = index_[key];
	partners.push_back(RelationshipEntry(partner, frequency));
}

const std::vector<RelationshipEntry>& RelationshipIndex::getEntries(const Word* key) const
{
	static const std::vector<RelationshipEntry> empty;
	auto it = index_.find(key);
	if (it == index_.end()) {
		return empty;
	}

	return it->second;
}

Relationship::Relationship(const Word* left, const Word* right, int frequency) : left_(left), right_(right), frequency_(frequency)
{


}

const Word* Relationship::getLeft() const
{
	return left_;
}

const Word* Relationship::getRight() const
{
	return right_;
}

int Relationship::getFrequency() const
{
	return frequency_;
}
