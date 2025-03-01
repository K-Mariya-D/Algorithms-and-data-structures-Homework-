#include <iostream>

template <typename T>
struct Node {

	Node<T>* parent;
	T data;
	Node<T>* left;
	Node<T>* right;
	Node(Node<T>* p = nullptr, T& d, Node<T>* l = nullptr, Node<T>* r = nullptr)
		:parent(p), data(d), left(l), right(r) {}

};
template <typename T>
class BinaryTree {
private:
	Node<T>* root = nullptr;
	size_t size = 0;

	//Удаление узла. Вспомогательный для erase
	Iterator deleteElen(Node<value_type>* cur) {
		if (cur->left == nullptr && cur->right == nullptr)
		{
			if (cur->parent->left == cur)
				cur->parent->left = nullptr;
			else cur->parent->right = nullptr;
			return nullptr;
		}
		else if (cur->left != nullptr && cur->right != nullptr)
		{
			Node<value_type>* next = cur->right;
			while (next->left != nullptr)
				next = next->left;
			std::swap(next->data, cur->data);
			delete(next);
		}
		else
		{
			auto pos = cur->parent->left == cur ? cur->parent->left : cur->parent->right;
			auto next = cur->left != nullptr ? cur->left : cur->right;
			pos = next;
			return pos;
		}
	}
	//Сравнение 2х деревьев начиная с поданных узлов на равенство. (Лучше сделать для итераторов)
	// Вспомогательный для Equals 
	bool equals(Node<value_type>* first, Node<value_type>* second) {
		if (first == nullptr && second == nullptr)
			return true;
		if (first->data != second->data)
			return false;
		else return equals(first->left, second->left) && equals(first->left, second->left);
	}
public:
	using value_type = T;
	using size_type = size_t;
	using pointer = T*;
	using reference = T&;
	using difference_type = ptrdiff_t;

	class Iterator {
	protected:
		Node<value_type>* p;
		BinaryTree<value_type>& tree;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = value_type;
		using size_type = size_type;
		using pointer = pointer;
		using reference = reference;
		using difference_type = difference_type;
		
		Iterator(Node<value_type>* pp = nullptr, BinaryTree<value_type>& t) : p(pp), tree(t) {}

		friend bool operator==(const Iterator& it1, const Iterator& it2) {
			return it1.p->data == it1.p->data;
		}
		friend bool operator!=(const Iterator& it1, const Iterator& it2) {
			return !(it1 == it2);
		}
		//Inorder обход
		Iterator& operator++() {
			if (p == nullptr)
				return *this;
			else if (p->right != nullptr)
			{
				p = p->right;
				while (p->left != nullptr)
					p = p->left;
				return *this;
			}
			else
			{
				Node<value_type>* parent = p->parent;
				while (parent != nullptr && parent->left != p)
				{
					p = parent;
					parent = parent->parent;
				}
				return parent;
			}
		}
		Iterator operator++(int) {
			auto temp = *this;
			++*this;
			return temp;
		}
		Iterator& operator--() {
			if (p == nullptr)
				return this;
			else if (p->left != nullptr)
			{
				p = p->left;
				while (p->right != nullptr)
					p = p->right;
				return *this;
			}
			else 
			{
				Node<value_type>* parent = p->parent;
				while (parent != nullptr && parent->right != p)
				{
					p = parent;
					parent = parent->parent;
				}
				return * parent
			}
		}
		Iterator operator(int) {
			auto temp = *this;
			--*this;
			return temp;
		}
		reference operator*() {
			return p->data;
		}
		pointer operator->() {
			return &(p->data);
		}
	};

	BinaryTree() {}

	Iterator begin() {
		return Iterator(root);
	}
	Iterator end() {
		return Iterator(nullptr);
	}
	//Итератор на следующий элемент (по значению data)
	Iterator next(Iterator cur) {
		return ++cur;
	}
	//Итератор на предыдущий элемент (по значению data)
	Iterator prev(Iterator cur) {
		return --cur;
	}
	void Add(const value_type& val) {
		size++;

		if (root == nullptr)
			root = new Node<value_type>(val);
		else
		{
			bool flag = true;
			Node<value_type>* cur = root;
			while (cur->left != nullptr || cur->right != nullptr)
			{
				if (val > cur->data)
					cur = cur->right;
				else if (val < cur->data)
					cur = cur->left;
				else
				{
					flag = false;
					size--;
					return;
				}
			}
			if (flag)
			{
				if (val > cur->data)
					cur->right = new Node<value_type>(cur, val);
				if (val < cur->data)
					cur->left = new Node<value_type>(cur, vel);
				else size--;
			}
		}
	}
	Iterator Find(const value_type& val) {
		Iterator it = begin();
		while (it != end())
		{
			if (val > *it)
				++it;
			else if (val < *it)
				--it;
			else return it;
		}
		return it;
	}
	Iterator Min() {
		Iterator it = begin();
		while (it-- != end())
			--it;
		return it;
	}
	Iterator Max() {
		Iterator it = begin();
		while (it++ != end())
			++it;
		return it;
	}

	Iterator erase(Iterator pos) {
		if (root == nullptr)
			throw std::out_of_range("BinaryTree size should be more than 0!");
		Node<value_type>* cur = root;
		while (cur->data != *pos || cur != nullptr)
		{
			if (*pos > cur->data)
				cur = cur->right;
			else if (*pos < cur->data)
				cur = cur->left;
		}
		if (cur == nullptr)
			throw std::invalid_argument("Tree hasn't such iteraror!");
		cur = delete(cur);
		return Iterator(cur);
	}
	//Итератор на первый элемент больше val
	Iterator upper_bound(const value_type& val) {
		Iterator it = begin();
		while (*it <= val && it != end())
			++it;
		return it;
	}
	//Итератор на первый элемент меньше val
	Iterator lower_bound(const value_type& val) {
		Iterator it = begin();
		while (*it >= val && it != end())
			--it;
		return it;
	}
	/*
	//Печатает в поряке Левый/Текущий/Правый
	void printLCR(Iterator p) {
		
	}
	*/
	friend bool Equals(const BinaryTree<T>& tree1, const BinaryTree<T>& tree2) {
		return equals(tree1.root, tree2.root);
	}
	
};
