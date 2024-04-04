#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <set>
#include <algorithm>

int Parent(int v, int* parent) {
    if (parent[v] != v) {
        parent[v] = Parent(parent[v], parent);
    }
    return parent[v];
}

void Union(int v1, int v2, int*parent) {
    parent[Parent(v1, parent)] = v2;
}

int kraskala(std::vector<std::pair<int, std::pair<int, int>>>& graph, std::set<std::vector<int>>& mst, int n, int m) {
    int* parent = new int[n];

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    int result = 0;

    for (int i = 0; i < m; ++i) {
        int v1 = graph[i].second.first, v2 = graph[i].second.second, weight = graph[i].first;

        if (Parent(v1, parent) != Parent(v2, parent)) {
            Union(v1, v2, parent);

            result += weight;
            
            mst.insert({weight, v1, v2});
        }
    }

    delete[] parent;

    return result;
}

int kraskala(std::vector<std::pair<int, std::pair<int, int>>>& graph, int n, int m, std::vector<int>& nv_edge) {
    int* parent = new int[n];

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    int result = 0;

    for (int i = 0; i < m; ++i) {
        int v1 = graph[i].second.first, v2 = graph[i].second.second, weight = graph[i].first;

        if (Parent(v1, parent) != Parent(v2, parent) && !((weight == nv_edge[0]) && (v1 == nv_edge[1]) && (v2 == nv_edge[2]))) {
            Union(v1, v2, parent);

            result += weight;
        }   
    }

    delete[] parent;

    return result;
}

void solve() {
    int n, m;
    std::cin >> n >> m;
    
    std::vector<std::pair<int, std::pair<int, int>>> graph;

    for (int i = 0; i < m; ++i) {
        int start, end, weight;
        std::cin >> start >> end >> weight;

        graph.push_back({weight, {start - 1, end - 1}});
    }

    std::sort(graph.begin(), graph.end());


    std::set<std::vector<int>> mst;

    int result1 = kraskala(graph, mst, n, m);

    int result2 = -1;

    for (auto e : mst) {
        std::set<std::vector<int>> ms;
        int tmp = kraskala(graph, n, m, e);

        if (result2 == -1 || tmp < result2) {
            result2 = tmp;
        }
    }

    std::cout << result1 << " " << result2;
}

// int main() {
//     solve();
//     return 0;
// }