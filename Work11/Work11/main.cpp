#include "graph.h"
#include <cassert>


int main()
{
	setlocale(LC_ALL, "Russian");
	
	std::cout << "Неориентированный связный граф: " << std::endl;
	std::vector<Edge> edges = std::vector<Edge>{
		Edge(0, 1, 7), Edge(0, 2, 9), Edge(0, 5, 14),
		Edge(1, 2, 10), Edge(1, 3, 15), Edge(2, 3, 11),
		Edge(2, 5, 2), Edge(5, 4, 9), Edge(4, 3, 6)
	};
	
	Graph gr1 = Graph(edges, 6);
	
	gr1.Print();

	std::cout << "Минимальные пути по алг. Дейкстры: " << std::endl;
	int from = 0;

	std::vector<int> res = gr1.Deikstra(from);

	assert(res[0] == 0); assert(res[3] == 20);
	assert(res[1] == 7); assert(res[4] == 20);
	assert(res[2] == 9); assert(res[5] == 11);

	for (int i = 0; i < res.size(); i++)
		std::cout << "from: "<< from << " to: " << i << " -> " << res[i] << std::endl;
	std::cout << "<-------------------------------------------------------->" << std::endl;

	std::cout << "Ориентированный связный граф: " << std::endl;
	edges = std::vector<Edge>{
		Edge(0, 1, 7), Edge(0, 2, 9), Edge(5, 0, 14),
		Edge(1, 2, 10), Edge(1, 3, 15), Edge(2, 3, 11),
		Edge(2, 5, 2), Edge(4, 5, 9), Edge(3, 4, 6)
	};

	Graph gr2 = Graph(edges, 6, true);

	gr2.Print();

	std::cout << "Минимальные пути по алг. Дейкстры: " << std::endl;

	res = gr2.Deikstra(from);

	assert(res[0] == 0); assert(res[3] == 20);
	assert(res[1] == 7); assert(res[4] == 26);
	assert(res[2] == 9); assert(res[5] == 11);

	for (int i = 0; i < res.size(); i++)
		std::cout << "from: " << from << " to: " << i << " -> " << res[i] << std::endl;
	std::cout << "<-------------------------------------------------------->" << std::endl;

	std::cout << "Ориентированный несвязный граф: " << std::endl;
	edges = std::vector<Edge>{
		Edge(0, 1, 7), Edge(0, 2, 9), Edge(5, 0, 14),
		Edge(1, 2, 10), Edge(1, 3, 15), Edge(2, 3, 11),
		Edge(2, 5, 2), Edge(4, 5, 9), Edge(3, 4, 6),
		Edge(6, 7, 2), Edge(7, 8, 4), Edge(8, 6, 3)
	};

	Graph gr3 = Graph(edges, 9, true);

	gr3.Print();

	std::cout << "Минимальные пути по алг. Дейкстры: " << std::endl;

	res = gr3.Deikstra(from);

	assert(res[0] == 0); assert(res[3] == 20); assert(res[6] == INT_MAX);
	assert(res[1] == 7); assert(res[4] == 26); assert(res[7] == INT_MAX);
	assert(res[2] == 9); assert(res[5] == 11); assert(res[8] == INT_MAX);

	for (int i = 0; i < res.size(); i++)
		std::cout << "from: " << from << " to: " << i << " -> " << res[i] << std::endl;

}