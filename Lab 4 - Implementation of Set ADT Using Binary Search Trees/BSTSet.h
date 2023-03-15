#ifndef BSTSET_H_
#define BSTSET_H_

#include "TNode.h"
#include <vector>

class BSTSet
{
private:
	// must contain only one private field
	TNode* root = NULL;

public:
	// required constructors/destructor
	BSTSet();
	BSTSet(const std::vector<int>& input);
	~BSTSet();

	// required methods
	bool isIn(int v);
	void add(int v);

	const TNode* finMin(TNode* target_node);
	void removeRec(int e, TNode* &node_target);
	bool remove(int v);
	void UnionRec(TNode* node_target);
	void Union(const BSTSet& s);
	void intersectionRec(TNode* node_target, BSTSet& input_tree, BSTSet& this_tree);
	void intersection(const BSTSet& s);
	
	void differenceRec(TNode* node_target);
	void difference(const BSTSet& s);

	int sizeRec(TNode* node_target);
	int size();
	
	int height();
	int heightRec(TNode* node_target);

	void printNonRec(); // create and use class MyStack
	void deleteRec(TNode* target_node); // create and use class MyStack

	// provided recursive print method
	void printBSTSet();

	// Used for testing
	TNode* getRoot()
	{
		return root;
	}

private:
	// provided helper methods
	void printBSTSet(TNode* t);
};

#endif /* BSTSET_H_ */
