#include <vector>
#include <sstream>

template <typename T, template <typename, typename> class Container = std::vector, typename Comparator = std::less<T>>
class BinaryHeap {
	Container<T, std::allocator<T>> vect;
	Comparator cmp;

	//Опускает вершину на нужную позицию
	void heapify(int i) {
		while (true) 
		{
			int l = 2 * i + 1;
			int r = 2 * i + 2;
			auto largest = i;

			if (vect.size() > l && cmp(vect[largest], vect[l]))
				largest = l;
			if (vect.size() > r && cmp(vect[largest], vect[r]))
				largest = r;
			if (largest == i)
				break;

			auto temp = vect[i];
			vect[i] = vect[largest];
			vect[largest] = temp;
			i = largest;
		}
	}
public:
	using value_type = typename T;
	using size_type = typename size_t;

	BinaryHeap(): vect(), cmp() {}
	BinaryHeap(size_type size): cmp() {
		vect = Container<T, std::allocator<T>>(size);
	}
	BinaryHeap(const Container<T, std::allocator<T>>& source): cmp()
	{
		vect = source;
		for (size_type i = vect.size() / 2; i >= 0; i--)
		{
			heapify(i);
		}
	}
	//Сливает 2е кучи в одну
	BinaryHeap& Concatenate(const BinaryHeap<value_type, Container, Comparator>& heap) {
		for (auto elem : heap.vect)
			push(elem);
		return *this;
	}
	void push(value_type val) {
		vect.push_back(val);
		int j = vect.size() - 1; //текущий
		int i = (j - 1) / 2; //родитель

		while (i >= 0  && cmp(vect[i], vect[j]))
		{
			auto temp = vect[i];
			vect[i] = vect[j];
			vect[j] = temp;
			j = i;
			i = (j - 1) / 2;
		}
	}
	value_type top() {
		if (vect.empty()) throw std::out_of_range("Heap is empty");
		return vect.front();
	}
	value_type pop() {
		if (vect.empty()) throw std::out_of_range("Heap is empty");

		auto res = vect[0];
		vect[0] = vect.back();
		vect.pop_back();
		heapify(0);
		return res;
	}
	friend std::ostream& operator<<(std::ostream& os, const BinaryHeap<value_type, Container, Comparator>& heap) {
		for (int i = 0; i < heap.vect.size(); i++)
			os << heap.vect[i] << " ";
		return os;
	}

};