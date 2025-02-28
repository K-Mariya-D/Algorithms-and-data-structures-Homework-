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
	/*
	Iterator erase(Iterator pos) {
		if (root == nullptr)
			throw std::out_of_range("BinaryTree size should be more than 0!");
		Node<value_type>* cur = root;

	}*/
};