#include <iostream>
#include "graph.h"


int main()
{
    string start_point;
    std::cin >> start_point; std::cin.ignore();
    string end_point;
    std::cin >> end_point; std::cin.ignore();
    int n;
    std::cin >> n; std::cin.ignore();

    //—читывание всех остановок в объекты StepArea
    unordered_map<string, StopArea> areas{};
    for (int i = 0; i < n; i++) {
        string stop_name;
        std::getline(std::cin, stop_name);
        vector<int> ind;
        int pos = 0;
        while (stop_name.find(',', pos) != string::npos)
        {
            ind.push_back(stop_name.find(',', pos));
            pos = stop_name.find(',', pos) + 1;
        }
        areas[stop_name.substr(9, ind[0] - 9)] =
        { stop_name.substr(9, ind[0] - 9),
        stop_name.substr(ind[0] + 2, ind[1] - ind[0] - 3),
        stof(stop_name.substr(ind[2] + 1, ind[3] - ind[2] - 1)),
        stof(stop_name.substr(ind[3] + 1, ind[4] - ind[3] - 1)) };
    }
    int m;
    std::cin >> m; std::cin.ignore();

    //‘ормирование ребер графа 
    vector<pair<StopArea, StopArea>> edges{};
    for (int i = 0; i < m; i++) {
        string route;
        std::getline(std::cin, route);
        int ind1 = route.find(':');
        int ind2 = route.find(':', ind1 + 1);
        string id1 = route.substr(ind1 + 1, ind2 - ind1 - 10);
        string id2 = route.substr(ind2 + 1, route.length() - ind2 - 1);
        edges.push_back({ areas[id1], areas[id2] });
    }
    Graph gr = Graph(edges);

    std::vector<string> res = gr.Deikstra(areas[start_point.substr(9, start_point.length() - 9)],
        areas[end_point.substr(9, end_point.length() - 9)]);
    if (res.size() == 1 && start_point != end_point)
        std::cout << "IMPOSSIBLE" << endl;
    else
        for (string s : res)
            std::cout << s << std::endl;
}