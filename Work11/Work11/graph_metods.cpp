#include "graph.h"
#include <queue>

void Graph::Print()
{
	std::cout << "vertex_1 -> {vertex_2, w}, where w is weight between vertex_1 and vertex_2" << std::endl;
	for (int i = 0; i < adjList.size(); i++)
	{
		std::cout << i << " -> ";
		for (Vertex v : adjList[i])
			std::cout << "{" << v.vert << ", " << v.path << "} ";
		std::cout << std::endl;
	}
}

std::vector<int> Graph::Deikstra(int from)
{
	struct cmp
	{
		bool operator() (const Vertex& v1, const Vertex& v2) const
		{
			return v1.path > v2.path;
		}
	};

	//какие вершины уже посещены
	std::vector<bool> visit = std::vector<bool>(adjList.size(), false);

	//минимальный путь к каждой вершине
	std::vector<int> minWeight = std::vector<int>(adjList.size(), INT_MAX);
	minWeight[from] = 0;
	
	//вершины, следующие в очереди на посещение
	std::priority_queue<Vertex, std::vector<Vertex>, cmp> que;
	que.push({ from, 0 });

	while (!que.empty())
	{
		int i = que.top().vert;
		que.pop();

		if (visit[i]) continue;

		//Обновляем метки
		for (Vertex v : adjList[i])
		{
			if (visit[v.vert]) continue;
			
			if (minWeight[i] + v.path < minWeight[v.vert])
			{
				minWeight[v.vert] = minWeight[i] + v.path;
				//добавляем вершину в очередь
				que.push({v.vert, minWeight[v.vert]});
			}
		}
		//Отмечаем вершину посещённой
		visit[i] = true;
	}

	return minWeight;
}