#pragma once

//  Фрагменты для реализации сбалансированных деревьев поиска - заготовка, не рабочая, доделать

#include <iostream>
#include <cassert>
#include <queue>
#include <vector>
#include <string>
#include <iterator>
#include <memory>
#include <memory_resource>
#include <initializer_list>
#include <functional>
#include <exception>

template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class Binary_Search_Tree
{
	//  Объект для сравнения ключей. Должен удовлетворять требованию строго слабого порядка, т.е. иметь свойства:
	//    1. Для любого x => cmp(x,x) == false (антирефлексивность)
	//    2. Если cmp(x,y) == true  =>  cmp(y,x) == false (асимметричность)
	//    3. Если cmp(x,y) == cmp(y,z) == true  =>  cmp(x,z) == true  (транзитивность)
	//    4. Если cmp(x,y) == cmp(y,z) == false  =>  cmp(x,z) == false  (транзитивность несравнимости)
	//  Этим свойством обладает, к примеру, оператор <. Если нужно использовать оператор <= , который не обладает
	//     нужными свойствами, то можно использовать его отрицание и рассматривать дерево как инвертированное от требуемого.
	Compare cmp = Compare();

	//  Узел бинарного дерева, хранит ключ, три указателя и признак nil для обозначения фиктивной вершины
	class Node
	{
	public:  //  Все поля открыты (public), т.к. само определение узла спрятано в private-части дерева
		//  Возможно, добавить поле типа bool для определения того, является ли вершина фиктивной (листом)
		Node* parent;
		Node* left;
		Node* right;
		//  Хранимый в узле ключ
		T data;
		//Признак isNill
		bool isNill;
		Node(T value = T(), Node* p = nullptr, Node* l = nullptr, Node* r = nullptr, bool nill = false) : parent(p), data(value), left(l), right(r), isNill(nill) {}
	};

	//  Стандартные контейнеры позволяют указать пользовательский аллокатор, который используется для
	//  выделения и освобождения памяти под узлы (реализует замену операций new/delete). К сожалению, есть 
	//  типичная проблема – при создании дерева с ключами типа T параметром шаблона традиционно указывается
	//  std::allocator<T>, который умеет выделять память под T, а нам нужен аллокатор для выделения/освобождения
	//  памяти под Node, т.е. std::allocator<Node>. Поэтому параметр шаблона аллокатора нужно изменить
	//  с T на Node, что и делается ниже. А вообще это одна из самых малополезных возможностей - обычно мы
	//  пользовательские аллокаторы не пишем, это редкость.

	//  Определяем тип аллокатора для Node (Allocator для T нам не подходит)
	using AllocType = typename std::allocator_traits<Allocator>::template rebind_alloc < Node >;
	//  Аллокатор для выделения памяти под объекты Node
	AllocType Alc;
	
	//  Рекурсивное клонирование дерева (не включая фиктивную вершину)
	//  Идея так себе - вроде пользуемся стандартной вставкой, хотя явное клонирование вершин было бы лучше
	void clone(Node * from, Node * other_dummy)
	{
		if (from == other_dummy)
			return;
		//	клонирование через insert? оно же будет переразвешиваться
		// Это ещё и рекурсивный проход в ширину, выраждает дево в список
		insert(from->data);	
		clone(from->right, other_dummy);
		clone(from->left, other_dummy);
	}
public:
	//  Эти типы должен объявлять контейнер - для функциональности
	using key_type = T;
	using key_compare = Compare;
	using value_compare = Compare;
	using value_type = typename T;
	using allocator_type = typename AllocType;
	using size_type = typename size_t;
	using difference_type = typename size_t;
	using pointer = typename T *;
	using const_pointer = typename const pointer;
	using reference = value_type & ;
	using const_reference = const value_type &;
	//using iterator = typename _Mybase::iterator;   //  Не нужно! Явно определили iterator внутри данного класса
	class iterator;   //  Предварительное объявление класса iterator, т.к. он определён ниже
	using const_iterator = iterator;
	class reverse_iterator;
	using const_reverse_iterator = reverse_iterator;

private:
	// Указательно на фиктивную вершину
	Node* dummy;

	//  Количесто элементов в дереве
	size_type tree_size = 0;

	// Создание фиктивной вершины - используется только при создании дерева
	inline Node* make_dummy()
	{
		// Выделяем память по размеру узла без конструирования
		dummy = Alc.allocate(1);
		
		//  Все поля, являющиеся указателями на узлы (left, right, parent) инициализируем и обнуляем
		std::allocator_traits<AllocType>::construct(Alc, &(dummy->parent));
		dummy->parent = dummy;

		std::allocator_traits<AllocType>::construct(Alc, &(dummy->left));
		dummy->left = dummy;

		std::allocator_traits<AllocType>::construct(Alc, &(dummy->right));
		dummy->right = dummy;
		
		std::allocator_traits<AllocType>::construct(Alc, &(dummy->isNill));
		dummy->isNill = true;
		//  Возвращаем указатель на созданную вершину
		return dummy;
	}

	// Создание узла дерева 
	template <typename T>
	inline Node* make_node(T&& elem, Node * parent, Node* left, Node* right)
	{
		// Создаём точно так же, как и фиктивную вершину, только для поля данных нужно вызвать конструктор
		Node * new_node = Alc.allocate(1);
		
		//  Все поля, являющиеся указателями на узлы (left, right, parent) инициализируем и обнуляем
		std::allocator_traits<AllocType>::construct(Alc, &(new_node->parent));
		new_node->parent = parent;

		std::allocator_traits<AllocType>::construct(Alc, &(new_node->left));
		new_node->left = left;

		std::allocator_traits<AllocType>::construct(Alc, &(new_node->right));
		new_node->right = right;

		//  Конструируем поле данных
		std::allocator_traits<AllocType>::construct(Alc, &(new_node->data), std::forward<T>(elem));
		//*new_node = elem;

		std::allocator_traits<AllocType>::construct(Alc, &(new_node->isNill));
		new_node->isNill = false;
		//  Возвращаем указатель на созданную вершину
		return new_node;
	}

	// Удаление фиктивной вершины
	inline void delete_dummy(Node* node) {
		std::allocator_traits<AllocType>::destroy(Alc, &(node->parent));
		std::allocator_traits<AllocType>::destroy(Alc, &(node->left));
		std::allocator_traits<AllocType>::destroy(Alc, &(node->right));
		std::allocator_traits<AllocType>::destroy(Alc, &(node->isNill));
		std::allocator_traits<AllocType>::deallocate(Alc, node, 1);
	}
	
	// Удаление вершины дерева
	inline void delete_node(Node * node) {
		//  Тут удаляем поле данных (вызывается деструктор), а остальное удаляем так же, как и фиктивную
		std::allocator_traits<AllocType>::destroy(Alc, &(node->data));
		delete_dummy(node);
		tree_size--;
	}

public:
	//  Класс итератора для дерева поиска
	class iterator 
	{
		friend class Binary_Search_Tree;
	protected:
		//  Указатель на узел дерева
		Node* data;

		explicit iterator(Node* d) : data(d) {	}
		
		//  Указатель на узел дерева
		inline Node* &_data()
		{
			return data;
		}
		inline bool isNill()
		{
			return data->isNill; 
		}
		// Родительский узел дерева
		inline iterator Parent() const noexcept
		{
			return iterator(data->parent);
		}
		// Левый дочерний узел (если отсутствует, то фиктивная вершина)
		inline iterator Left() const noexcept
		{
			return iterator(data->left);
		}
		// Правый дочерний узел (если отсутствует, то фиктивная вершина)
		inline iterator Right() const noexcept
		{
			return iterator(data->right);
		}
		// Является ли узел дерева левым у своего родителя
		inline bool IsLeft() const noexcept
		{
			Node* p = data->parent;
			return p->left == data;
		}
		// Является ли узел дерева правым у своего родителя
		inline bool IsRight() const noexcept
		{
			Node* p = data->parent;
			return p->right == data;
		}
		// Поиск «самого левого» элемента (относительно текущего)
		iterator GetMin() {
			Node* p = data;
			while (!p->left->isNill)
				p = p->left;
			return iterator(p);
		}
		// Поиск «самого правого» элемента (относительно текущего)
		iterator GetMax() {
			Node* p = data;
			while (!p->right->isNill)
				p = p->right;
			return iterator(p);
		}
	public:
		//  Определяем стандартные типы в соответствии с требованиями стандарта к двунаправленным итераторам
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Binary_Search_Tree::value_type;
		using difference_type = Binary_Search_Tree::difference_type;
		using pointer = Binary_Search_Tree::pointer;
		using reference = Binary_Search_Tree::reference;

		//  Значение в узле, на который указывает итератор
		inline const T& operator*() const
		{
			return data->data;
		}
		inline const T operator->() const
		{
			return data->data;
		}
		// Преинкремент - следующий элемент множества
		iterator & operator++()
		{
			if (data->isNill)
			{
				iterator begin = iterator(data->parent);
				data = begin.GetMax().data;
			}
			else if (!data->right->isNill)
			{
				data = data->right;
				while (!data->left->isNill)
					data = data->left;
			}
			else
			{
				Node* parent = data->parent;
				while (!parent->isNill && parent->left != data)
				{
					data = parent;
					parent = parent->parent;
				}
				data = parent;
			}
			return *this;
		}
		// Предекремент - переход на предыдущий элемент множества
		iterator & operator--()
		{
			if (data->isNill)
			{
				iterator begin = iterator(data->parent);
				data = begin.GetMax().data;
			}
			else if (!data->left->isNill)
			{
				data = data->left;
				while (!data->right->isNill)
					data = data->right;
			}
			else
			{
				Node* parent = data->parent;
				while (!parent->isNill && parent->right != data)
				{
					data = parent;
					parent = parent->parent;
				}
				data = parent;
			}
			return *this;
		}
		// Постинкремент
		iterator operator++(int) {
			auto temp = *this;
			++*this;
			return temp;
		}
		// Постдекремент
		iterator operator--(int) {
			auto temp = *this;
			--*this;
			return temp;
		}
		
		friend bool operator != (const iterator & it_1, const iterator & it_2)
		{
			return !(it_1 == it_2);
		}

		friend bool operator == (const iterator & it_1, const iterator & it_2)
		{
			return (const_cast<iterator&>(it_1))._data() == (const_cast<iterator&>(it_2))._data();
		}
		
		//  Эти операции не допускаются между прямыми и обратными итераторами
		const iterator & operator=(const reverse_iterator& it) = delete;
		bool operator==(const reverse_iterator& it) = delete;
		bool operator!=(const reverse_iterator& it) = delete;
		iterator(const reverse_iterator& it) = delete;
	};
	class reverse_iterator
	{
		friend class Binary_Search_Tree;
	protected: 
		iterator data;
	public :
		explicit reverse_iterator(iterator it) : data(it) {	};

		//  Определяем стандартные типы в соответствии с требованиями стандарта к двунаправленным итераторам
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Binary_Search_Tree::value_type;
		using difference_type = Binary_Search_Tree::difference_type;
		using pointer = Binary_Search_Tree::pointer;
		using reference = Binary_Search_Tree::reference;

		//  Значение в узле, на который указывает итератор
		inline const T& operator*() const {
			return *data;
		}
		reverse_iterator& operator++() {
			--data;
			return *this;
		}
		reverse_iterator& operator--() {
			++data;
			return *this;
		}
		reverse_iterator operator++(int) {
			auto temp = *this;
			--*this;
			return temp;
		}
		reverse_iterator operator--(int) {
			auto temp = *this;
			++*this;
			return temp;
		}
		friend bool operator != (const reverse_iterator& it_1, const reverse_iterator& it_2) {
			return !(it_1 == it_2);
		}

		friend bool operator == (const reverse_iterator& it_1, const reverse_iterator& it_2) {
			return it_1.data == it_2.data;
		}
	};
	//Самый левый элемент
	iterator begin() const noexcept {
		iterator begin = iterator(dummy->parent);
		if (begin.isNill())
			return end();
		/*
		iterator next = --begin;
		while (!next.isNill())
		{
			begin = next;
			--next;
		}*/
		return begin.GetMin();
	}
	iterator end() const noexcept { return iterator(dummy);  }
	//Самый правый элемент
	reverse_iterator rbegin() const	noexcept { 
		iterator begin = iterator(dummy->parent);
		if (begin.isNill())
			return rend();
		/*
		iterator next = ++begin;
		while (!next.isNill())
		{
			begin = next;
			++next;
		}*/
		return reverse_iterator(begin.GetMax());
	}
	reverse_iterator rend() const noexcept { return reverse_iterator(end()); }

	Binary_Search_Tree(Compare comparator = Compare(), AllocType alloc = AllocType())
		: dummy(make_dummy()), cmp(comparator), Alc(alloc) {}

	Binary_Search_Tree(std::initializer_list<T> il) : dummy(make_dummy())
	{
		for (const auto &x : il)
			insert(x);
	}

	AllocType get_allocator() const noexcept { return Alc; }
	key_compare key_comp() const noexcept { return cmp; }
	value_compare value_comp() const noexcept { return cmp; }

	inline bool empty() const noexcept { return tree_size == 0; }

private:
	template <class RandomIterator, typename T = typename std::iterator_traits<RandomIterator>::value_type>
	typename std::enable_if<
		std::is_same<
		typename std::iterator_traits<RandomIterator>::iterator_category,
		std::random_access_iterator_tag
		>::value
	>::type
	ordered_insert(RandomIterator first, RandomIterator last, iterator position) {
		if (first >= last) return;
		RandomIterator center = first + (last - first)/2 ;
		iterator new_pos = insert(position, *center);  //  итератор на вставленный элемент
		ordered_insert(first, center, position);
		ordered_insert(center + 1, last, ++position);
	}

public:
	template <class InputIterator>
	Binary_Search_Tree(InputIterator first, InputIterator last, Compare comparator = Compare(), AllocType alloc = AllocType()) : dummy(make_dummy()), cmp(comparator), Alc(alloc)
	{
		//  Проверка - какой вид итераторов нам подали на вход
		if (std::is_same<typename std::iterator_traits<InputIterator>::iterator_category, typename std::random_access_iterator_tag>::value) {
			//  Если итератор произвольного доступа, то есть надежда, что диапазон отсортирован
			//    а даже если и нет - не важно, всё равно попробуем метод деления пополам для вставки
			std::vector<T> temp_vec(first, last);
			ordered_insert(temp_vec.begin(), temp_vec.end(), end());
		}
		else
			std::for_each(first, last, [this](T x) { insert(std::move(x)); });
	}

	Binary_Search_Tree(const Binary_Search_Tree & tree) : dummy(make_dummy())
	{	//  Размер задаём
		tree_size = tree.tree_size;
		if (tree.empty()) return;

		dummy->parent = recur_copy_tree(tree.dummy->parent, tree.dummy);
		dummy->parent->parent = dummy;

		//  Осталось установить min и max
		dummy->left = iterator(dummy->parent).GetMin()._data();
		dummy->right = iterator(dummy->parent).GetMax()._data();
	}

	private:

    //  Рекурсивное копирование дерева
	Node* recur_copy_tree(Node * source, const Node * source_dummy) 
	{
		//  Сначала создаём дочерние поддеревья
		Node* left_sub_tree;
		if (source->left != source_dummy)
			left_sub_tree = recur_copy_tree(source->left, source_dummy);
		else
			left_sub_tree = dummy;

		Node* right_sub_tree;
		if (source->right != source_dummy)
			right_sub_tree = recur_copy_tree(source->right, source_dummy);
		else
			right_sub_tree = dummy;
		
		//  Теперь создаём собственный узел
		Node* current = make_node(source->data, nullptr, left_sub_tree, right_sub_tree);
		//  Устанавливаем родителей
		if (source->right != source_dummy)
			current->right->parent = current;
		if (source->left != source_dummy)
			current->left->parent = current;
		//  Ну и всё, можно возвращать
		return current;
	}

	public:
	const Binary_Search_Tree & operator=(const Binary_Search_Tree &tree)
	{
		if (this == &tree) return *this;
		
		Binary_Search_Tree tmp{tree};
		swap(tmp);
		
		return *this;
	}

	size_type size() const { return tree_size; }

	// Обмен содержимым двух контейнеров
	void swap(Binary_Search_Tree & other) noexcept {
		std::swap(dummy, other.dummy);

		//  Обмен размера множеств
		std::swap(tree_size, other.tree_size);
	}

	// Вставка элемента по значению. 
	std::pair<iterator, bool> insert(const T & value)
	{
		Node* curent = dummy;
		Node* next = curent->parent;
		bool flag = true;
		while (!next->isNill)
		{
			curent = next;
			if (cmp(next->data, value))
				next = next->right;
			else if (cmp(value, next->data))
				next = next->left;
			else 
			{ 
				flag = false;
				break; 
			}
		}
		if (flag)
		{
			Node* new_node = make_node(value, curent, dummy, dummy);
			if (tree_size == 0)
				dummy->parent = new_node;
			else 
			{
				if (cmp(value, curent->data))
					curent->left = new_node;
				else curent->right = new_node;
			}
			tree_size++;
			return std::make_pair(iterator(new_node), flag);
		}
		else return std::make_pair(iterator(curent), flag);
	}	
	//???
	iterator insert(const_iterator position, const value_type& x) {
		//  Проверяем, корректно ли задана позиция для вставки: ... prev -> x -> position -> ...
		//  2 5 6 7 10 11 15,    x = 8
		return insert(std::move(x)).first;
	}

	//  Не самый лучший вариант.
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while (first != last) insert(*first++);
	}

	iterator find(const value_type& value) const { 
		
		iterator current = iterator(dummy->parent); 

		while (!current.isNill())
		{
			if (!cmp(*current, value) && !cmp(value, *current))
				return current;
			else if (cmp(*current, value))
				++current;
			else --current;
		}
		return current;
	}
	//поднимается от самого маленького
	iterator lower_bound(const value_type& key) {
		iterator it = ++begin();
		while (*it <= key && it != end())
			++it;
		return it == end()? rbegin().data: --it;
	}

	const_iterator lower_bound(const value_type& key) const {
		return const_iterator(const_cast<Binary_Search_Tree *>(this)->lower_bound(key));
	}
	//опускается от самого большого
	iterator upper_bound(const value_type& key) {
		iterator it = --rbegin().data;
		while (*it > key && it != end())
			--it;
		return it == end()? begin(): ++it;
	}

	const_iterator upper_bound(const value_type& key) const {
		return const_iterator(const_cast<Binary_Search_Tree*>(this)->upper_bound(key));
	}

	size_type count(const value_type& key) const {
		return find(key) != end() ? 1 : 0;
	}
	
	// Что он делает? (Возвращает диапозон элементов с одинаковым ключом, но таких ведь не может быть в БДП?)
	std::pair<const_iterator, const_iterator> equal_range(const value_type& key) const {
		return std::make_pair(lower_bound(key), upper_bound(key));
	}

protected:
	//!!! Удаление листа дерева. Возвращает количество удалённых элементов
	size_type delete_leaf(iterator leaf) {
		#ifdef _DEBUG
		if (leaf.isNill()) return 0; // Стоит потом убрать, так как уже проверяем, что итератор валидный в erase
		#endif
		iterator parent = leaf.Parent(); 
		delete_node(leaf.data);
		Node* p = parent.data;
		if (p->left == leaf.data) {
			p->left = dummy;
		}
		else if (p->right == leaf.data) {
			p->right = dummy;
		}
		return 1;
	}

public:
	//!!!  Удаление элемента, заданного итератором. Возвращает количество удалённых элементов (для set - 0/1)
	iterator erase(iterator elem) {
		//  Если фиктивный элемент, то ошибка - такого не должно происходить
		if (elem.isNill()) return iterator(elem);

		if (elem.Left().isNill() && elem.Right().isNill())
		{
			delete_leaf(elem);
			return end();
		}
		else if (!elem.Left().isNill() && !elem.Right().isNill())
		{
			iterator next = elem.Right().GetMin();
			std::swap(elem.data->data, next.data->data);
			return erase(next);
		}
		else 
		{
			Node* next = !elem.Left().isNill()? elem.Left().data : elem.Right().data;
			Node* deleted = elem.data;
			//std::swap(elem.data->data, next.data->data);
			//return erase(next);
			if (deleted->parent->isNill)
			{
				next->parent = dummy;
				dummy->parent = next;
			}
			else if (elem.IsRight())
			{
				deleted->parent->right = next;
				next->parent = deleted->parent;
			}
			else
			{
				deleted->parent->left = next;
				next->parent = deleted->parent;
			}
			delete_node(deleted);
			return iterator(next);
		}
	}
	
	size_type erase(const value_type& elem) {
		iterator it = find(elem); 
		if (it.isNill())
			return 0;
		erase(it);
		return 1;
	}

	iterator erase(const_iterator first, const_iterator last) {
		while (first != last)
			first = erase(first);
		return last;
	}

	//Если передавать по ссылкам,все хорошо. Конструктор копий принескольких деревьях ломается.
	friend bool operator== (const Binary_Search_Tree<T> &tree_1, const Binary_Search_Tree<T> & tree_2)
	{
		auto i = tree_1.begin(), ii = tree_2.begin();
		for (; (i != tree_1.end()) && (ii != tree_2.end()); ++i, ++ii)
		{
			if (*i != *ii)
				return false;
		}
		return i == tree_1.end() && ii == tree_2.end();
	}

	//  Очистка дерева (без удаления фиктивной вершины)
	void clear() {
		Free_nodes(dummy->parent);
		tree_size = 0;
		dummy->parent = dummy->left = dummy->right = dummy;
	}

private:
	//  Рекурсивное удаление узлов дерева, не включая фиктивную вершину
	void Free_nodes(Node* node)
	{ 
		if (!node->isNill)
		{
			Free_nodes(node->left);
			Free_nodes(node->right);
			delete_node(node);
		}
	}
	
public:
	~Binary_Search_Tree()
	{
		clear(); // рекурсивный деструктор
		delete_dummy(dummy);
	}
};

template <class Key, class Compare, class Allocator>
void swap(Binary_Search_Tree<Key, Compare, Allocator>& x, Binary_Search_Tree<Key, Compare, Allocator>& y) noexcept(noexcept(x.swap(y))) {
	x.swap(y);
};


template <class Key, class Compare, class Allocator>
bool operator==(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	typename Binary_Search_Tree<Key, Compare, Allocator>::const_iterator it1{ x.begin() }, it2{ y.begin() };
	while (it1 != x.end() && it2 != y.end() && *it1 == *it2) {
		++it1; ++it2;
	}

	return it1 == x.end() && it2 == y.end();
}

template <class Key, class Compare, class Allocator>
bool operator<(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	
	typename Binary_Search_Tree<Key, Compare, Allocator>::const_iterator it1{ x.begin() }, it2{ y.begin() };
	while (it1 != x.end() && it2 != y.end() && *it1 == *it2) {
		++it1; ++it2;
	}

	if (it1 == x.end())
		return it2 != y.end();
	
	return it2 != y.end() && *it1 < *it2;
}

template <class Key, class Compare, class Allocator>
bool operator!=(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	return !(x == y);
}

template <class Key, class Compare, class Allocator>
bool operator>(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	return y < x;
}

template <class Key, class Compare, class Allocator>
bool operator>=(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	return !(x<y);
}

template <class Key, class Compare, class Allocator>
bool operator<=(const Binary_Search_Tree<Key, Compare, Allocator>& x, const Binary_Search_Tree<Key, Compare, Allocator>& y) {
	return   !(y < x);
}



