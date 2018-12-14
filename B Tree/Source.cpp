#include <iostream>

#include "bTree.h"

int main() {
	BTree B(3);
	B.insert(10);
	B.insert(20);
	B.insert(5);
	B.insert(6);
	B.insert(12);
	B.insert(30);
	B.insert(7);
	B.insert(17);

	B.remove(12);

	B.traverse();

	return 0;
}