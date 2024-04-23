#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <set>
#include <algorithm>

int find(std::vector<int>& parent, int v) {
    if (parent[v] != v) {
        parent[v] = find(parent, parent[v]);
    }
    return parent[v];
}

void Union(std::vector<int>& parent, int v1, int v2) {
    parent[find(parent, v1)] = v2;
}

void dfs(std::vector<std::set<std::pair<int, int>>>& graph, int previous, int x, std::vector<int>& a, std::vector<int>& b, std::vector<int>& c) {
    for (auto nv : graph[x]) {
        if (nv.first != previous) {
            dfs(graph, x, nv.first, a, b, c);
            a[x] = std::max(a[x], b[nv.first] + nv.second - c[nv.first]);
            b[x] += c[nv.first];
        }
    }
    a[x] += b[x];
    c[x] = std::max(a[x], b[x]);
}

void kraskala(std::vector<std::pair<int, std::pair<int, int>>>& graph, std::vector<std::set<std::pair<int, int>>>& mst, int n, int m) {
    std::vector<int> parent(n);

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    for (int i = 0; i < m; ++i) {
        int v1 = graph[i].second.first, v2 = graph[i].second.second, weight = graph[i].first;
        if (find(parent, v1) != find(parent, v2)) {
            Union(parent, v1, v2);

            mst[v1].insert({v2, weight});
            mst[v2].insert({v1, weight});
        }
    }

    parent.clear();
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

    std::sort(graph.begin(), graph.end(), [](std::pair<int, std::pair<int, int>> v1, std::pair<int, std::pair<int, int>> v2) {return v1.first < v2.first;});

    std::vector<std::set<std::pair<int, int>>> mst(n);

    kraskala(graph, mst, n, m);

    // for (int i = 0; i < n; ++i) {
    //     std::cout << "[" << i << "] : ";
    //     for (auto nv : mst[i]) {
    //         std::cout << "{" <<  nv.first << ", " << nv.second << "}, ";
    //     }
    //     std::cout << '\n';
    // }

    std::vector<int> a(n, 0);
    std::vector<int> b(n, 0);
    std::vector<int> c(n, 0);

    int root;

    for (int i = 0; i < n; ++i) {
        if (mst[i].size() == 1) {
            root = i;
            break;
        }
    }

    std::vector<bool> used(n, false);
    dfs(mst, -1, root, a, b, c);
    std::cout << c[root];
}

// int main() {
//     solve();
//     return 0;
// }
