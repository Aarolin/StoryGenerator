#pragma once

#include "Word.h"
#include <unordered_map>
#include <vector>


class Relationship {
public:
	Relationship(const Word* left, const Word* right, int frequency);

	const Word* getLeft() const;
	const Word* getRight() const;
	int getFrequency() const;

private:
	const Word* left_;
	const Word* right_;
	int frequency_;
};

class RelationshipEntry {
public:
	RelationshipEntry(const Word* partner, int frequency);
	const Word* getPartner() const;
	int getFrequency() const;

private:
	const Word* partner_;
	int frequency_;
};

class RelationshipIndex {
public:

	RelationshipIndex();

	void addEntry(const Word* key, const Word* partner, int frequency);
	const std::vector<RelationshipEntry>& getEntries(const Word* key) const;

private:
	std::unordered_map<const Word*, std::vector<RelationshipEntry>, WordHash, WordPtrEqual> index_;
};