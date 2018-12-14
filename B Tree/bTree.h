#pragma once

#include <vector>

class Node {
	int maxK;
	bool leaf;
	int noK;
	std::vector<int> *keys;
	std::vector<Node *> *children;
public:
	Node(int maxK, bool leaf);
	~Node();
	void traverse();
	Node *search(int value);
	int findKey(int value);
	void insertNonFull(int value);
	void splitChild(int index, Node *child);
	void remove(int value);
	void removeFromLeaf(int index);
	void removeFromNonLeaf(int index);
	int getPred(int index);
	int getSucc(int index);
	void fill(int index);
	void borrowFromPrev(int index);
	void borrowFromNext(int index);
	void merge(int index);

	friend class BTree;
};

class BTree {
	Node *root;
	int maxK;
public:
	BTree(int maxK);
	~BTree();
	void traverse();
	Node *search(int value);
	void insert(int value);
	void remove(int value);
};