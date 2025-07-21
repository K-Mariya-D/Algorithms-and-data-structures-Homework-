#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <queue>
#include "graph.h"

std::vector<string> Graph::Deikstra(StopArea from, StopArea to)
{
    //���� � �������
    unordered_map<StopArea, StopArea, StopArea::HashFunct> edges;

    //����� ������� ��� ��������
    std::unordered_map<string, bool> visit{};
    for (auto pair : adjList)
        visit[pair.first.Id] = false;

    //����������� ���� � ������ �������
    std::unordered_map<string, float> minWeight;
    for (auto pair : adjList)
        minWeight[pair.first.Id] = LDBL_MAX;
    minWeight[from.Id] = 0;

    struct cmp
    {
        bool operator()(StopAreaWithD& s1, StopAreaWithD& s2) const
        {
            return s1.d > s2.d;
        }
    };

    //�������, ��������� � ������� �� ���������
    priority_queue<StopAreaWithD, vector<StopAreaWithD>, cmp> que;
    que.push(StopAreaWithD(from, 0));

    while (!que.empty())
    {
        StopAreaWithD i = que.top();
        que.pop();

        //��������� �����
        for (StopAreaWithD v : adjList[(StopArea)i])
        {
            if (visit[v.Id]) continue;

            if (minWeight[i.Id] + v.d < minWeight[v.Id])
            {
                minWeight[v.Id] = minWeight[i.Id] + v.d;
                //��������� ������� � �������
                if (!visit[v.Id])
                    que.push(StopAreaWithD(v, minWeight[v.Id]));
                auto it = std::find_if(edges.begin(), edges.end(), [=](auto& e) {return e.first == (StopArea)v;});
                if (it != edges.end())
                    edges.erase(it);
                edges[(StopArea)v] = (StopArea)i;
            }
        }
        //�������� ������� ����������
        visit[i.Id] = true;
    }

    std::vector<string> res;
    StopArea path = to;

    //��� ���� �������?
    while (!(path.Id == from.Id && path.Name == from.Name))
    {
        if (edges.count(path) == 0)
            break;
        res.push_back(path.Name);
        path = edges[path];
    }
    res.push_back(from.Name);
    reverse(res.begin(), res.end());

    return res;
}