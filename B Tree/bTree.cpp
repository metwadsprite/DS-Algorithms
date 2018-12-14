#include <iostream>

#include "bTree.h"

Node::Node(int maxK, bool leaf) {
	this->maxK = maxK;
	this->leaf = leaf;
	this->keys = new std::vector<int>(2 * maxK - 1);
	this->children = new std::vector<Node *>(2 * maxK);
	this->noK = 0;
}

Node::~Node() {
	delete this->keys;
	for (auto &child : *this->children) {
		delete child;
	}
	delete this->children;
}

void Node::traverse() {
	int i = 0;
	for (; i != this->noK; ++i) {
		if (!leaf) {
			this->children->at(i)->traverse();
		}
		std::cout << this->keys->at(i) << ' ';
	}
	if (!leaf) {
		this->children->at(i)->traverse();
	}
}

Node *Node::search(int value) {
	int i = 0;
	while (i < this->noK && value > this->keys->at(i)) {
		++i;
	}
	if (this->keys->at(i) == value) {
		return this;
	}
	if (this->leaf) {
		return nullptr;
	}
	return this->children->at(i)->search(value);
}

void Node::insertNonFull(int value) {
	int i = this->noK - 1;
	if (leaf) {
		while (i >= 0 && this->keys->at(i) > value) {
			this->keys->at(i + 1) = keys->at(i);
			--i;
		}
		this->keys->at(i + 1) = value;
		this->noK += 1;
	}
	else {
		while (i >= 0 && this->keys->at(i) > value) {
			--i;
		}
		if (this->children->at(i + 1)->noK == 2 * this->maxK - 1) {
			this->splitChild(i + 1, this->children->at(i + 1));
			if (this->keys->at(i + 1) < value) {
				++i;
			}
		}
		this->children->at(i + 1)->insertNonFull(value);
	}
}

void Node::splitChild(int index, Node *child) {
	Node *newNode = new Node(child->maxK, child->leaf);
	newNode->noK = this->maxK - 1;

	for (int j = 0; j != this->maxK - 1; ++j) {
		newNode->keys->at(j) = child->keys->at(j + this->maxK);
	}

	if (!child->leaf) {
		for (int j = 0; j != this->maxK; ++j) {
			newNode->children->at(j) = child->children->at(j + this->maxK);
		}
	}

	child->noK = this->maxK - 1;

	for (int j = this->noK; j >= index + 1; --j) {
		this->children->at(j + 1) = this->children->at(j);
	}

	this->children->at(index + 1) = newNode;

	for (int j = this->noK - 1; j >= index; --j) {
		this->keys->at(j + 1) = this->keys->at(j);
	}

	this->keys->at(index) = child->keys->at(this->maxK - 1);
	this->noK += 1;
}

int Node::findKey(int value) {
	int index = 0;
	while (index < this->noK && this->keys->at(index) < value) {
		++index;
	}
	return index;
}

void Node::remove(int value) {
	int index = this->findKey(value);
	
	if (index < this->noK && this->keys->at(index) == value) {
		if (this->leaf) {
			this->removeFromLeaf(index);
		}
		else {
			this->removeFromNonLeaf(index);
		}
	}
	else {
		if (leaf) {
			return;
		}

		bool flag = ((index == this->noK) ? true : false);

		if (this->children->at(index)->noK < maxK) {
			this->fill(index);
		}

		if (flag &&  index > this->noK) {
			this->children->at(index - 1)->remove(value);
		}
		else {
			this->children->at(index)->remove(value);
		}
		return;
	}
}

void Node::removeFromLeaf(int index) {
	for (int i = index + 1; i != this->noK; ++i) {
		this->keys->at(i - 1) = this->keys->at(i);
	}

	--this->noK;
}

void Node::removeFromNonLeaf(int index) {
	int value = this->keys->at(index);

	if (this->children->at(index)->noK >= this->maxK) {
		int pred = this->getPred(index);
		this->keys->at(index) = pred;
		this->children->at(index)->remove(pred);
	}
	else {
		this->merge(index);
		this->children->at(index)->remove(value);
	}
	return;
}

int Node::getPred(int index) {
	Node *current = this->children->at(index);

	while (!current->leaf) {
		current = current->children->at(current->noK - 1);
	}

	return current->keys->at(current->noK - 1);
}

int Node::getSucc(int index) {
	Node *current = this->children->at(index + 1);

	while (!current->leaf) {
		current = current->children->at(0);
	}

	return current->keys->at(0);
}

void Node::fill(int index) {
	if (index and this->children->at(index - 1)->noK >= this->maxK) {
		this->borrowFromPrev(index);
	}
	else if (index != this->noK && this->children->at(index + 1)->noK >= this->maxK) {
		this->borrowFromNext(index);
	}
	else {
		if (index != this->noK) {
			this->merge(index);
		}
		else {
			this->merge(index - 1);
		}
	}
}

void Node::borrowFromPrev(int index) {
	Node *child = this->children->at(index);
	Node *sibling = this->children->at(index - 1);

	for (int i = child->noK - 1; i != 0; --i) {
		child->keys->at(index + 1) = child->keys->at(index);
	}

	if (!child->leaf) {
		for (int i = child->noK; i != 0; --i) {
			child->children->at(index + 1) = child->children->at(index);
		}
	}

	child->keys->at(0) = this->keys->at(index - 1);

	if (!child->leaf) {
		child->children->at(0) = sibling->children->at(sibling->noK);
	}

	this->keys->at(index - 1) = sibling->keys->at(sibling->noK - 1);

	child->noK += 1;
	sibling->noK -= 1;
}

void Node::borrowFromNext(int index) {
	Node *child = this->children->at(index);
	Node *sibling = this->children->at(index + 1);

	child->keys->at(child->noK) = this->keys->at(index);

	if (!child->leaf) {
		child->children->at(child->noK + 1) = sibling->children->at(0);
	}

	this->keys->at(index) = sibling->keys->at(0);

	for (int i = 1; i != sibling->noK; ++i) {
		sibling->keys->at(i - 1) = sibling->keys->at(i);
	}

	if (!sibling->leaf) {
		for (int i = 1; i != sibling->noK; ++i) {
			sibling->children->at(i - 1) = sibling->children->at(i);
		}
	}

	child->noK += 1;
	sibling->noK -= 1;
}

void Node::merge(int index) {
	Node *child = this->children->at(index);
	Node *sibling = this->children->at(index + 1);

	child->keys->at(this->maxK - 1) = this->keys->at(index);

	for (int i = 0; i != sibling->noK; ++i) {
		child->keys->at(i + this->maxK) = sibling->keys->at(i);
	}

	if (!child->leaf) {
		for (int i = 0; i <= sibling->noK; ++i) {
			child->children->at(i + this->maxK) = sibling->children->at(i);
		}
	}

	for (int i = index + 1; i != this->noK; ++i) {
		this->keys->at(i - 1) = this->keys->at(i);
	}

	for (int i = index + 2; i <= this->noK; ++i) {
		this->children->at(i - 1) = this->children->at(i);
	}

	child->noK += sibling->noK + 1;
	--this->noK;
	delete sibling;
}

BTree::BTree(int maxK) {
	this->root = nullptr;
	this->maxK = maxK;
}

BTree::~BTree() {
	delete this->root;
}

void BTree::traverse() {
	if (this->root) {
		this->root->traverse();
	}
}

Node *BTree::search(int value) {
	return (this->root == nullptr) ? nullptr : root->search(value);
}

void BTree::insert(int value) {
	if (!this->root) {
		this->root = new Node(this->maxK, true);
		this->root->keys->at(0) = value;
		this->root->noK = 1;
		return;
	}
	if (this->root->noK == 2 * this->maxK - 1) {
		Node *newNode = new Node(this->maxK, false);
		newNode->children->at(0) = this->root;
		newNode->splitChild(0, this->root);
		int i = 0;
		if (newNode->keys->at(0) < value) {
			++i;
		}
		newNode->children->at(i)->insertNonFull(value);
		this->root = newNode;
	}
	else {
		this->root->insertNonFull(value);
	}
}

void BTree::remove(int value) {
	if (!this->root) {
		return;
	}

	this->root->remove(value);

	if (this->root->noK == 0) {
		Node *temp = this->root;
		
		if (this->root->leaf) {
			this->root = nullptr;
		}
		else {
			this->root = this->root->children->at(0);
		}

		delete temp;
	}
}