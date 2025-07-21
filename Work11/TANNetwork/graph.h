#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct StopArea
{
    string Id;
    string Name;
    long double latitude; //широта
    long double longitude; //долгота

    struct HashFunct
    {
        size_t operator()(const StopArea& sa) const { return hash<string>()(sa.Id); }
    };
    bool operator==(const StopArea& other) const { return Id == other.Id; }
};
class Graph
{
private:
    struct StopAreaWithD : StopArea
    {
        long double d;
        StopAreaWithD(StopArea sa, long double D)
        {
            Id = sa.Id;
            Name = sa.Name;
            latitude = sa.latitude;
            longitude = sa.longitude;
            d = D;
        }
    };

    //stopArea -> map<{stopArea1, d1},  {stopArea2, d2}, ...>
    unordered_map<StopArea, std::vector<StopAreaWithD>, StopArea::HashFunct> adjList;
public:
    Graph(vector<pair<StopArea, StopArea>> edges)
    {
        for (auto pair : edges)
        {
            StopArea A = pair.first;
            StopArea B = pair.second;
            long double x = (B.longitude - A.longitude) * cos((A.latitude + B.latitude) / 2);
            long double y = (B.latitude - A.latitude);
            long double d = sqrt(x * x + y * y) * 6371;
            adjList[A].push_back(StopAreaWithD(B, d));
        }
    }
    std::vector<string> Deikstra(StopArea from, StopArea to);
};