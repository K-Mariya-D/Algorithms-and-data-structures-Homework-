#include <iostream>
#include "BinaryTree.h"
#include <chrono>

int main()
{
	setlocale(LC_ALL, "Russian");

	BinaryTree<int> tree;
	tree.Add(8);
	tree.Add(6);
	tree.Add(10);
	tree.Add(1);
	tree.Add(5);
	tree.Add(13);
	tree.Add(2);
	tree.Add(19);
	std::cout << "����� ������ � ������� �����/�������/������:" << std::endl;
	tree.printLCR();
	std::cout << "\n����� ������ � ������� ������/�������/�����:" << std::endl;
	tree.printRCL();
	std::cout << "\n����� ������ �� �����:" << std::endl;
	tree.printBySlaces();


	std::cout << "\n������������ ������� ������ = " << *tree.Max() << std::endl;
	std::cout << "����������� ������� ������ = " << *tree.Min() << std::endl;

	std::cout << "��������� ������� ������ 6 = " << *tree.upper_bound(6) << std::endl;
	std::cout << "��������� ������� ������ 6 = " << *tree.lower_bound(6) << std::endl;

	using namespace std::chrono;
	std::cout << "\n������ ���������� �� ������ ������:" << std::endl;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	BinaryTree<int> bt = EratostheneTree(12000);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "����� ������ � �� ��� n = 12000: " << duration << std::endl;

	std::cout << "\n������ ���������� �� ������ �������:" << std::endl;
	t1 = high_resolution_clock::now();
	std::vector<int> vect = EratostheneArray(12000);
	t2 = high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "����� ������ � �� ��� n = 12000: " << duration << std::endl;
}