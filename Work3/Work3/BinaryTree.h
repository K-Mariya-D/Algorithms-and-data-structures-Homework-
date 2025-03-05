#include <iostream>
#include <stack>
#include <queue>
#include <vector>

template <typename T>
struct Node {

	Node<T>* parent;
	T data;
	Node<T>* left;
	Node<T>* right;
	Node(T d, Node<T>* p = nullptr, Node<T>* l = nullptr, Node<T>* r = nullptr)
		:parent(p), data(d), left(l), right(r) {}

};

template <typename T>
class BinaryTree {
private:
	Node<T>* root = nullptr;
	size_t size = 0;

	//Печатает в поряке Левый/Текущий/Правый. Вспомогательный для printLCR
	void printLCR(Node<T>* p) {
		if (p->left != nullptr)
			printLCR(p->left);
		std::cout << p->data << " ";
		if (p->right != nullptr)
			printLCR(p->right);
	}
	//Удаление узла. Вспомогательный для erase
	Node<T>* deleteElem(Node<T>* cur) {
		if (cur->left == nullptr && cur->right == nullptr)
		{
			if (cur->parent->left == cur)
				cur->parent->left = nullptr;
			else cur->parent->right = nullptr;
			delete cur;
			return nullptr;
		}
		else if (cur->left != nullptr && cur->right != nullptr)
		{
			Node<T>* next = cur->right;
			while (next->left != nullptr)
				next = next->left;
			cur->data = next->data; 

			if (next->parent->left == next)
				next->parent->left = deleteElem(next);
			else next->parent->right = deleteElem(next);
			
			return cur;
		}
		else
		{
			Node<T>* next = cur->left != nullptr ? cur->left : cur->right;
			if (cur->parent->left != nullptr && cur->parent->left->data == cur->data)
				cur->parent->left = next;
			else
				cur->parent->right = next;
			
			next->parent = cur->parent;
			delete cur;
			return next;
		}
	}
	//Сравнение 2х деревьев начиная с поданных узлов на равенство. (Лучше сделать для итераторов)
	// Вспомогательный для Equals 
	bool equals(Node<T>* first, Node<T>* second) {
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
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = value_type;
		using size_type = size_type;
		using pointer = pointer;
		using reference = reference;
		using difference_type = difference_type;
		
		Iterator(Node<value_type>* pp = nullptr) : p(pp) {}

		friend bool operator==(const Iterator& it1, const Iterator& it2) {
			if (it1.p == nullptr || it2.p == nullptr)
				return it1.p == it2.p;
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
				this->p = parent;
				return *this;
			}
		}
		Iterator operator++(int) {
			auto temp = *this;
			++*this;
			return temp;
		}
		Iterator& operator--() { 
			if (p == nullptr)
				return *this;
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
				this->p = parent;
				return *this;
			}
		}
		Iterator operator--(int) {
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
			while (true) {
				if (val > cur->data)
				{
					if (cur->right != nullptr)
						cur = cur->right;
					else
					{
						cur->right = new Node<value_type>(val, cur);
						break;
					}

				}
				else if (val < cur->data)
				{
					if (cur->left != nullptr)
						cur = cur->left;
					else 
					{
						cur->left = new Node<value_type>(val, cur);
						break;
					}
				}
				else 
				{
					size--;
					break;
				}
			}
		}
	}
	Iterator Find(const value_type& val) {
		Node<value_type>* p = root;
		while (p != nullptr)
		{
			if (val > p->data)
				p = p->right;
			else if (val < p->data)
				p = p->left;
			else return p;
		}
		return Iterator(p);
	}
	Iterator Min() {
		Node<value_type>* p = root;
		while (p->left != nullptr)
			p = p->left;
		return Iterator(p);
	}
	Iterator Max() {
		Node<value_type>* p = root;
		while (p->right != nullptr)
			p = p->right;
		return Iterator(p);
	}

	Iterator erase(Iterator pos) {
		if (root == nullptr)
			throw std::out_of_range("BinaryTree size should be more than 0!");
		Node<value_type>* cur = root;
		while (cur != nullptr && cur->data != *pos)
		{
			if (*pos > cur->data)
				cur = cur->right;
			else if (*pos < cur->data)
				cur = cur->left;
		}
		if (cur == nullptr)
			throw std::invalid_argument("Tree hasn't such iteraror!");
		cur = deleteElem(cur);
		size--;
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

	void printLCR() {
		printLCR(root);
	}
	void printRCL() {
		std::stack<Node<value_type>*> st;
		st.push(root);
		while (!st.empty())
		{
			Node<value_type>* temp = st.top();
			while (temp->right != nullptr)
			{
				st.push(temp->right);
				temp = st.top();
			}
			while (!st.empty())
			{
				if (temp->left == nullptr)
				{
					std::cout << temp->data << " ";
					st.pop();
					if (st.empty())
						break;
					else temp = st.top();
				}
				else
				{
					std::cout << temp->data << " ";
					st.pop();
					st.push(temp->left);
					break;
				}
			}	
		}
	}
	void printBySlaces() {
		std::queue<Node<value_type>*> que;
		Node<value_type>* p = root;
		que.push(p);
		while (!que.empty())
		{
			p = que.front();
			que.pop(); 
			std::cout << p->data << " ";
			if (p->left != nullptr)
				que.push(p->left);
			if (p->right != nullptr)
				que.push(p->right);
		}
	}
	friend bool Equals(const BinaryTree<T>& tree1, const BinaryTree<T>& tree2) {
		return equals(tree1.root, tree2.root);
	}
	~BinaryTree() {
		std::queue<Node<value_type>*> que;
		que.push(root);
		while (!que.empty())
		{
			Node<value_type>* val = que.front();
			if (val->left != nullptr)
			{
				que.push(val->left);
				val->left->parent = nullptr;
			}
			if (val->right != nullptr)
			{
				que.push(val->right);
				val->right->parent = nullptr;
			}
			que.pop();
			delete val;
			val = nullptr;
		}
	}
};

//Решето Эротосфена на основе дерева
BinaryTree<int>& EratostheneTree(const int& n);
//Решето Эротосфена на основе массива 
std::vector<int> EratostheneArray(const int& n);
