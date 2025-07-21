#include "HashTable.h"
#include <cstring>
#include <unordered_set>
#include "AVLTree.h"
#include <set>

int main()
{
	
	HashTable<int> table = HashTable<int>(10000);
	std::unordered_multiset<int> set = std::unordered_multiset<int>(10000);
	AVL_Tree<int> tree = AVL_Tree<int>(); 

	//Сравнение работы по времени (insert)
	time_t start = clock();
	for (int i = 0; i < 10000; i++)
		table.insert(i);
	std::cout.precision(20);
	std::cout << "HashTable time insert: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 0; i < 10000; i++)
		set.insert(i);
	std::cout.precision(20);
	std::cout << "Multiset time insert: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 0; i < 10000; i++)
		tree.insert(i);
	std::cout.precision(20);
	std::cout << "AVLTree time insert: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n" <<std::endl;

	//Сравнение работы по времени (erase)
	start = clock();
	for (int i = 0; i < 10000; i++)
		table.erase(i);

	std::cout.precision(20);
	std::cout << "HashTable time erase: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 0; i < 10000; i++)
		set.erase(i);
	std::cout.precision(20);
	std::cout << "Multiset time erase: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 0; i < 10000; i++)
		tree.erase(i);
	std::cout.precision(20);
	std::cout << "AVLTree time erase: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n" << std::endl;

	//Проверка корректоности работы хэш-функции
	HashTable<int> table1 = HashTable<int>(2000);
	srand(time(0));

	for (int i = 0; i < 1000; i++)
		table1.insert(std::rand());

	std::cout << "SIZE:" << table1.size() << std::endl;
	//table1.print();

	std::vector<int> vect{};
	for (int i = 0; i < 100; i++)
		vect.push_back(std::rand());
	
	start = clock();
	for (int j = 0; j < 100; j++)
	{
		for (int i: vect)
			table1.insert(i);
		std::cout << "Pass " << j << ": number of collisions during insertion: " << table1.CollisionCounter() << std::endl;
		table1.SetCollisionCounter();

		table1.find(vect[rand() % vect.size()]);
		std::cout << "Pass " << j << ": number of collisions during finding: " << table1.CollisionCounter() << std::endl;
		table1.SetCollisionCounter();

		for (int i : vect)
			table1.erase(i);
	}
	std::cout.precision(20);
	std::cout << "Test time for HashTable: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n" << std::endl;
	//table1.print();

	//Сравнение с AVLTree
	std::set<int> set1{};

	for (int i = 0; i < 1000; i++)
		set1.insert(std::rand());

	start = clock();
	for (int j = 0; j < 100; j++)
	{
		for (int i : vect)
			set1.insert(i);

		set1.find(vect[rand() % vect.size()]);

		for (int i : vect)
			set1.erase(i);
	}
	std::cout.precision(20);
	std::cout << "Test time for Set: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";


}