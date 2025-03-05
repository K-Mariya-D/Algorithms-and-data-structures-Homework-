#include "BinaryTree.h"

BinaryTree<int>& EratostheneTree(const int& n) {
	BinaryTree<int> tree;
	for (int i = 2; i <= n ; i++)
		tree.Add(i);
	
	BinaryTree<int>::Iterator p = tree.begin();
	BinaryTree<int>::Iterator cur = tree.Find(pow(*p, 2));
	while (cur != tree.end())
	{	
		for (auto val = *cur; val <= n; val += *p )
		{
			auto temp = tree.Find(val);
			if (temp != tree.end())
				tree.erase(temp);
			
		}
		++p;
		cur = tree.Find(pow(*p, 2));
	}
	return tree;
}

std::vector<int> EratostheneArray(const int& n) {
	std::vector<int> vect = std::vector<int>(n - 1);
	for (int i = 2; i <= n; i++)
		vect[i - 2] = i;

	int ind = 0;
	while (ind < vect.size()) {
		auto p = vect[ind];
		for (int i = pow(p, 2); i <= n; i += p) {
			std::vector<int>::iterator it = std::find(vect.begin(), vect.end(), i);
			if (it != vect.end())
				vect.erase(it);
			
		}
		ind++;
	}
	return vect;
}