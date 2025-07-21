#include <vector>
#include <algorithm>
#include <sstream>
#include "BinaryHeap.h"

template <typename T, template <typename, typename> class Container = std::vector, typename Comparator = std::less<T>>
class PriorityQueue {
    Container<T, std::allocator<T>> elems; 
    Comparator cmp; 

public:
	using key_type = T;
	using key_compare = Comparator;
	using value_compare = Comparator;
	using value_type = typename T;
	using allocator_type = typename std::allocator<T>;
	using size_type = typename size_t;
	using difference_type = typename size_t;
	using pointer = typename T*;
	using const_pointer = typename const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;

	PriorityQueue():elems() {}
	PriorityQueue(size_type sz) {
		elems = Container<T, std::allocator<T>>(sz);
	}
	//Создание очереди из массива элементов (выполнить сортировку);
	PriorityQueue(Container<T, std::allocator<T>> cont) {
		//elems = cont.sort();
		elems.assign(cont.begin(), cont.end());
		std::sort(elems.begin(), elems.end(), cmp);
	}

	size_type size() { return elems.size(); }

    void push(const value_type& val) {
		
        for (int i = 0; i < elems.size(); i++)
			if (cmp(elems[i], val))
			{
				elems.insert(elems.begin() + i, val);
				return;
			}
		if (elems.size() == 0)
		{
			elems.push_back(val);
			return;
		}
		elems.push_back(val);
    }
	value_type top() {
		return elems.front();
	}

    value_type pop() {
		value_type elem = elems.front();
		elems.erase(elems.begin());
		return elem;
    }

    bool empty() const {
        return elems.empty();
    }
	//Слияние двух очередей с приоритетами;
	//Сливает вторую очередь в первую и возвращает её
	PriorityQueue& Concatenate(const PriorityQueue<value_type, Container, Comparator>& queue) {
		for (auto elem: queue.elems)
			push(elem);
		return *this;
	}
	friend std::ostream& operator<<( std::ostream& os, const PriorityQueue<value_type, Container, Comparator>& queue) {
		for (auto elem: queue.elems)
			os << elem << " ";
		return os;
	}
};

template <typename T, template <typename, typename> class Container = std::vector, typename Comparator = std::less<T>>
class PriorityQueueWithHeap {
	BinaryHeap<T, Container, Comparator> heap;
	Comparator cmp;

public:
	using key_type = T;
	using key_compare = Comparator;
	using value_compare = Comparator;
	using value_type = typename T;
	using allocator_type = typename std::allocator<T>;
	using size_type = typename size_t;
	using difference_type = typename size_t;
	using pointer = typename T*;
	using const_pointer = typename const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	
	PriorityQueueWithHeap() :heap() {}
	PriorityQueueWithHeap(size_type sz) {
		heap = BinaryHeap<T, Container, Comparator>(sz);
	}

	//Создание очереди из массива элементов (выполнить сортировку);
	PriorityQueueWithHeap(const Container<T, std::allocator<T>>& cont) {
		heap = BinaryHeap<T, Container, Comparator>(cont);
	}
	
	size_type size() { return heap.size(); }


	void push(const value_type& val) {
		heap.push(val);		
	}
	value_type top() {
		return heap.top();
	}

	value_type pop() {
		return heap.pop();
	}

	bool empty() const {
		return heap.size() == 0;
	}
	//Слияние двух очередей с приоритетами;
	//Сливает вторую очередь в первую и возвращает её
	friend PriorityQueueWithHeap& Concatenate(const PriorityQueueWithHeap<value_type, Container, Comparator>& queue) {
		heap.Concatenate(queue.heap);
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os, const PriorityQueueWithHeap<value_type, Container, Comparator>& queue) {
		return queue.heap.operator<<(os);
	}
};