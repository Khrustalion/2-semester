#include <iostream>
//#include <blazingio.hpp>
#include <set>
#include <vector>
#include <queue>

int dfs(std::vector<std::set<std::pair<int, int>>>& graph, int s, int e, int min_edge, bool* visited) {
    visited[s] = true;

    if (s == e) {
        return min_edge;
    }

    for (auto nv: graph[s]) {
        if (!visited[nv.first]) {
            if (min_edge == -1) min_edge = nv.second;
            return dfs(graph, nv.first, e, std::min(min_edge, nv.second), visited);
        }
    }
}

bool bfs(std::vector<std::vector<int>>& graph, int n) {
    std::vector<int> parent(n, -1);

    std::queue<int> q;
    q.push(0);

    bool find_path = false;

    while (q.size()) {
        int v = q.front();
        q.pop();

        for (int i = 1; i < n; ++i) {
            if (graph[v][i] != 0 && parent[i] == -1) {
                q.push(i);
                parent[i] = v;
            }
        }
    }

    if (parent[n-1] == -1) {
        return false;
    }

    int min_edge = -1;
    int parent_edge = parent[n-1];
    int cur_edge = n-1;

    while (parent_edge != -1) {
        min_edge = std::min(min_edge == -1 ? graph[parent_edge][cur_edge] : min_edge, graph[parent_edge][cur_edge]);
        cur_edge = parent_edge;
        parent_edge = parent[cur_edge];
    }

    parent_edge = parent[n-1];
    cur_edge = n-1;

    while (parent_edge != -1) {
        graph[parent_edge][cur_edge] -= min_edge;
        graph[cur_edge][parent_edge] += min_edge;
        cur_edge = parent_edge;
        parent_edge = parent[cur_edge];
    }

    // for (int i = 0; i < n; ++i) {
    //     for (int j = 0; j < n; ++j) {
    //         std::cout << graph[i][j] << " ";
    //     }
    //     std::cout << '\n';
    // }

    parent.clear();
    while (q.size()) {
        q.pop();
    }
    return true;
}

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));

    for (int i = 0; i < m; ++i) {
        int start, end, weight;

        std::cin >> start >> end >> weight;

        graph[start - 1][end - 1] = weight;
    }

    for (int i = 0; i < n; ++i) {
        graph[n-1][i] = 0;
    }

    while (bfs(graph, n)) {}

    int answer = 0;

    for (int i = 0; i < n; ++i) {
        answer += graph[n-1][i];
    }
    std::cout << answer;
}

// int main() {
//     solve();
//     return 0;
// }