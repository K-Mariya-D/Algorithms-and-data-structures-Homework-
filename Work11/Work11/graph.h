#include <vector>
#include <iostream>


//Структура ребра графа 
struct Edge
{
	int start;
	int end;
	int weight;
	Edge(int s, int e, int w = 1)
	{
		if (s < 0 || e < 0 || w < 0 || s == e)
			throw std::exception("invalid argument(s)");
		start = s;
		end = e;
		weight = w;
	}
};
class Graph
{
private:
	//Структура вершины графа
	struct Vertex
	{
		//вершина
		int vert;
		//длинна пути к ней 
		int path;
	};
	//Список вершин смежности графа
	std::vector<std::vector<Vertex>> adjList;
	bool oriented = false;
public :
	bool IsOriented() { return oriented; }
	//Через ребра графа
	Graph(std::vector<Edge> edges, int n, bool orient = false)
	{
		adjList.resize(n); // n - кол-во вершин

		if (!orient)
		{
			for (Edge e : edges)
			{
				adjList[e.start].push_back({ e.end, e.weight });
				adjList[e.end].push_back({ e.start, e.weight });
			}
		}
		else 
		{
			oriented = orient;
			for (Edge e : edges)
				adjList[e.start].push_back({ e.end, e.weight });
		}
	}
	void Print();
	std::vector<int> Deikstra(int from);
};

