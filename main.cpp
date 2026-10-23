#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <limits>
//#include <cmath>

template <typename T>
struct Edge {
    int vertex_id_from;
    int vertex_id_to;
    T weight;
};
//
//template <typename T>
//bool Equil(T x, T y){
//    return x < y;
//}
//template <>
//bool Equil(double x, double y){
//    double eps = 0.00001;
//    return abs(x-y) < eps;
//}

std::unordered_map<std::string, int> ReadVertex(std::istream& in = std::cin) {
    int count_vertex;
    std::cin >> count_vertex;

    std::unordered_map<std::string, int> name_to_ids;
    for (int i = 0; i < count_vertex; ++i) {
        std::string name;
        int id;
        std::cin >> name >> id;
        name_to_ids[name] = id;
    }
    return name_to_ids;
}

template <typename T>
std::vector<Edge<T>> ReadEdges(std::istream& in = std::cin){
    int count_edges;
    std::cin >> count_edges;

    std::vector<Edge<T>> edges(count_edges);
    for(int i = 0; i < count_edges; ++i){
        int vertex_id_from;
        int vertex_id_to;
        T weight;
        std::cin >> vertex_id_from >> vertex_id_to >> weight;
        edges[i] = { vertex_id_from, vertex_id_to, weight };
    }
    return edges;
}

template <typename T>
void Dump(const std::unordered_map<std::string, int>& name_to_ids, const std::vector<Edge<T>>& edges){
    std::cout << '\n';
    for(const auto& [k, v] : name_to_ids){
        std::cout << k << " - " << v << '\n';
    }

    std::cout << '\n';
    for(const Edge<T>& edge : edges){
        std::cout << edge.vertex_id_from << ", " << edge.vertex_id_to << ", " << edge.weight << '\n';
    }
}

template <typename T>
int GetVertexWithMinWeight(const std::unordered_map<int, T>& costs, const std::unordered_set<int>& processed){
    int vertex_id_min_weight = -1;
    T min_weight = std::numeric_limits<T>::max();
    for(const auto& [k, v] : costs){
        if(auto it = processed.find(k); it == processed.end()){
            if(v < min_weight){
                min_weight = v;
                vertex_id_min_weight = k;
            }
        }
    }
    return vertex_id_min_weight;
}

template <typename T>
void DijkstraPath(const std::unordered_map<std::string, int>& name_to_ids, const std::vector<Edge<T>>& edges){
    const std::string vertex_name_start = "start";
    const std::string vertex_name_finish = "finish";

    int vertex_id_start = name_to_ids.at(vertex_name_start);
    int vertex_id_finish = name_to_ids.at(vertex_name_finish);

    std::unordered_map<int, std::unordered_map<int, T>> graph;
    for(const auto& [k, v] : name_to_ids){
        graph[v] = {};
        if(vertex_id_finish != v){
            for(const Edge<T>& edge : edges){
                if(edge.vertex_id_from == v){
                    graph[v].insert({edge.vertex_id_to, edge.weight});
                }
            }
        }
    }

    std::unordered_map<int, T> costs;
    for(const auto& [k, v] : name_to_ids){
        costs[v] = std::numeric_limits<T>::max();
    }
    {
        const auto &vertex_to_start = graph.at(vertex_id_start);
        for (const auto &[id, w]: vertex_to_start) {
            costs[id] = w;
        }
    }

    std::unordered_map<int, int> parents;
    for(const auto& [k, v] : name_to_ids){
        if(v == vertex_id_start){
            const auto& vertex_to_start = graph.at(v);
            for(const auto& [id, w] : vertex_to_start){
                parents[id] = v;
            }
        } else {
            parents[v] = std::numeric_limits<T>::max();
        }
    }

    std::unordered_set<int> processed;
    int vertex_id_for_work = GetVertexWithMinWeight(costs, processed);
    while(vertex_id_for_work > 0){
        const T parent_cost = costs.at(vertex_id_for_work);
        const std::unordered_map<int, T>& edges_for_work = graph[vertex_id_for_work];
        for(const auto& [k, v] : edges_for_work){
            const T cost = costs.at(k);
            T new_cost = parent_cost + v;
            if(new_cost < cost){
                costs[k] = new_cost;
                parents[k] = vertex_id_for_work;
            }
        }
        processed.insert(vertex_id_for_work);
    }
    //Done!
    std::cout << '\n';
    std::cout << "result = " << costs.at(vertex_id_finish) << '\n';
}

int main() {
    std::unordered_map<std::string, int> name_to_ids = ReadVertex();
    std::vector<Edge<int>> edges = ReadEdges<int>();
    //Dump(name_to_ids, edges);
    DijkstraPath(name_to_ids, edges);
    return 0;
}

/*
4
start 1
A 2
B 3
finish 4
5
1 2 6
1 3 2
3 2 3
2 4 1
3 4 5
 */
