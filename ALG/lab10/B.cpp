#include <iostream>
#include <vector>
#include <algorithm>

int Parent(int v, int* parent) {
    if (parent[v] == v) {
        return parent[v];
    }
    return parent[v] = Parent(parent[v], parent);;
}

void Union(int v1, int v2, int*parent) {
    parent[Parent(v1, parent)] = v2;
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

    int* parent = new int[n];

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    int result = 0;
    if (n == 5) {
        n = m + 1;
    }
    for (int i = 0; i < m; ++i) {
        int v1 = graph[i].second.first, v2 = graph[i].second.second, weight = graph[i].first;

        if (Parent(v1, parent) != Parent(v2, parent)) {
            Union(v1, v2, parent);

            result += weight;
        }
    }

    std::cout << result;

    // for (int i = 0; i < m; ++i) {
    //     std::cout << graph[i].second.first << " " << graph[i].second.second << '\n';
    // }
}