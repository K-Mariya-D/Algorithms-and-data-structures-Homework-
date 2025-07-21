#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

template<
    class Key,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator<Key>
> class HashTable
{
private:
	struct Elem 
	{
		Key value;
		bool occupied = false;
		Elem() : value(Key()) {}
		Elem(const Key& val) :value(val) {}
	};
	
	std::vector<Elem> table;
	int h(const size_t& k) { return k % table.capacity(); }
	Hash hashfunct1 = Hash();
	KeyEqual comp = KeyEqual();
	int hashfunct2(const size_t& k, int i) { return (h(k) + i) % table.capacity(); }
	int collisionCounter = -1;

public:
    using key_type = Key;
    using value_type = Key;
    using size_type = typename size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;
	using difference_type = typename size_t;
	using allocator_type = typename Allocator;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename const pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = typename std::vector<Elem>::iterator;
	using const_iterator = const iterator;
    
	int CollisionCounter() { return collisionCounter; }
	void SetCollisionCounter() { collisionCounter = -1; }

	HashTable(int size) { table = std::vector<Elem>(size, Elem()); }
	HashTable(int size, Hash h, KeyEqual cmp) 
	{
		hashfunct1 = h;
		comp = cmp;
		table = std::vector<Elem>(size, Elem());
	}
	hasher hash_function() const { return hashfunct1; }

	//Если элементы есть, но они помеченны, как незанятые, то считает пустым
	bool empty() { return size() == 0; }
	int size() { return std::count_if(table.begin(), table.end(), [](Elem el) {return el.occupied == true;}); }
	void clear() { table.clear(); }
	std::pair<iterator, bool> insert(const value_type& val)
	{
		size_type key = hashfunct1(val);

		for (int i = 0; i < table.size(); i++)
		{
			collisionCounter++;
			int ind = hashfunct2(key, i);
			if (table[ind].occupied == false)
			{
				table[ind].value = val; 
				table[ind].occupied = true;
				return std::make_pair(table.begin() + ind, true);
			}
		}
		return std::make_pair(table.end(), false);
	}
	iterator erase(iterator pos)
	{   
		if (pos == table.end())
			throw std::invalid_argument("Invalid iterator");
		Elem el = *pos;
		el.occupied = false;
		return pos + 1;
	}
	iterator erase(const value_type& val)
	{   
		iterator pos = find(val);
		if (pos == table.end())
			throw std::invalid_argument("Such value don't exist");
		return erase(pos);
	}
	bool contains(const value_type& val) const
	{
		size_type key = hashfunct1(val);

		for (int i = 0; i < table.size(); i++)
		{
			int ind = hashfunct2(key, i);
			if (comp(table[ind].value, val) && table[ind].occupied == true)
			{
				return true;
			}
		}
		return false;
	}
	iterator find(const value_type& val)
	{
		size_type key = hashfunct1(val);

		for (int i = 0; i < table.size(); i++)
		{
			collisionCounter++;
			int ind = hashfunct2(key, i);
			if (comp(table[ind].value, val) && table[ind].occupied == true)
			{
				return table.begin() + ind;
			}
		}
		return table.end();
	}
	void print()
	{
		for (int i = 0; i < table.size(); i++)
		{
			Elem el = table[i];
			std::cout << "Index: " << i << ", Hash: " << h(el.value) << ", Value: " << el.value << " "<< el.occupied << std::endl;
		}
	}
};
