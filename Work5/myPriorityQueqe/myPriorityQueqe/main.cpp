#include <iostream>
#include "myPriorityQueue.h"
#include <ctime>
#include "fstream"
#include <queue>
#include <set>
#include <cstring>

struct LengthComparator {
	bool operator()(const std::string& a, const std::string& b) const {
		return a.size() < b.size(); // Сравниваем строки по длине
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");

	PriorityQueue<int> que1 = PriorityQueue<int>();
	std::priority_queue<int> que2 = std::priority_queue<int>();
	PriorityQueueWithHeap<int> que3 = PriorityQueueWithHeap<int>();

	//Сравнение работы по времени 
	time_t start = clock();
	for (int i = 1;i < 100000;++i)
	{
		que1.push(i);
	}
	std::cout.precision(20);
	std::cout << "Time push PriorityQueue with vector: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
	
	start = clock();
	for (int i = 1;i < 100000;++i)
	{
		que2.push(i);
	}
	std::cout.precision(20);
	std::cout << "Time push standard priority_queue: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 1;i < 100000;++i)
	{
		que3.push(i);
	}
	std::cout.precision(20);
	std::cout << "Time push PriorityQueue with heap: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
	std::cout << "<-------------------------------------------->" <<std::endl;
	
	for (int i = 1;i < 100000;++i)
	{
		que1.pop();
	}
	std::cout.precision(20);
	std::cout << "Time pop PriorityQueue with vector: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
	
	start = clock();
	for (int i = 1;i < 100000;++i)
	{
		que2.pop();
	}
	std::cout.precision(20);
	std::cout << "Time pop standard priority_queue: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";

	start = clock();
	for (int i = 1;i < 100000;++i)
	{
		que3.pop();
	}
	std::cout.precision(20);
	std::cout << "Time pop PriorityQueue with heap: " << double(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
	std::cout << "<-------------------------------------------->" << std::endl;
	
	PriorityQueue<std::string, std::vector, LengthComparator> que4;
	PriorityQueueWithHeap<std::string, std::vector, LengthComparator> que5;
	std::priority_queue<std::string, std::vector<std::string>, LengthComparator> que6;
	std::set<std::string> set;
	
	//Сравнение эффективности на примере поиска 20 самых длинных слов в "Войне и Мире"
	std::string word;
	time_t end;
	std::ifstream in("WarAndPeace.txt"); 
	if (in.is_open())
	{	
		while (in>>word)
		{
			if (word.back() == ',' || word.back() == '.' || word.back() == '!' || word.back() == '?')
				word.pop_back();
			std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
				return std::tolower(c); });
			set.insert(word);
		}
	}
	in.close();
	//1
	start = clock();
	for (std::string word : set)
		que5.push(word);
	end = clock();

	std::cout << "Text processing time:" << double(end - start) / CLOCKS_PER_SEC << " seconds\n";
	std::cout << "The 20 Longest Words in War and Peace (PriorityQueue with heap):" << std::endl;
	for (int i = 1; i <= 20; i++)
	{
		std::cout << i << ". " << que5.top() << std::endl;
		que5.pop();
	}
	
	//2
	start = clock();
	for (std::string word : set)
		que6.push(word);
	end = clock();

	std::cout << "\nText processing time:" << double(end - start) / CLOCKS_PER_SEC << " seconds\n";
	std::cout << "The 20 Longest Words in War and Peace (standard priority_queue):" << std::endl;
	for (int i = 1; i <= 20; i++)
	{
		std::cout << i << ". " << que6.top() << std::endl;
		que6.pop();
	}

	//3
	start = clock();
	for (std::string word : set)
		que4.push(word);
	end = clock();

	std::cout << "\nText processing time:" << double(end - start) / CLOCKS_PER_SEC << " seconds\n";
	std::cout << "The 20 Longest Words in War and Peace (PriorityQueue with vector):" << std::endl;
	for (int i = 1; i <= 20; i++)
	{
		std::cout << i << ". " << que4.top() << std::endl;
		que4.pop();
	}

}