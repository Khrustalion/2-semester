#include <iostream>
#include <vector>


void dfs(std::vector<int>* graph, int v, std::vector<int>& parent) {
    for (auto nv: graph[v]) {
        if (nv != parent[v]) {
            parent[nv] = v;
            dfs(graph, nv, parent);
        }
    }
}

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int>* graph = new std::vector<int>[n];

    for (int i = 0; i < m; ++i) {
        int start, end;

        std::cin >> start >> end;

        graph[start - 1].push_back(end - 1);
        graph[end - 1].push_back(start - 1);
    }

    std::vector<int> parent(n, -1);

    dfs(graph, n-1, parent);

    std::vector<int> degree(n, 0);

    int ptr = -1;

    for (int i = 0; i < n; ++i) {
        degree[i] = graph[i].size();

        if (degree[i] == 1 && ptr == -1) {
            ptr = i;
        }
    }

    int leaf = ptr;

    for (int i = 0; i < n - 2; ++i) {
        int next = parent[leaf];

        std::cout << next + 1 << " ";
        --degree[next];

        if (degree[next] == 1 && next < ptr) {
            leaf = next;
        } else {
            ++ptr;

            while (ptr < n && degree[ptr] != 1) {
                ++ptr;
            }
            leaf = ptr;
        }
    }
}
