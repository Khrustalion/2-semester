#include <iostream>
//#include <blazingio.hpp>
#include <set>
#include <vector>
#include <unordered_set>

bool dfs(std::vector<std::unordered_set<int>>& graph, std::vector<bool>& used, std::vector<bool>& used_tmp, int v, int cnt) {
    used_tmp[v] = true;
    for (auto nv : graph[v]) {
        if (!used_tmp[nv] && dfs(graph, used, used_tmp, nv, cnt + 1)) {
            used[v] = true;
            graph[v].extract(nv);
            graph[nv].insert(v);
            return true;
        }
    }
    if (graph[v].size() == 0 && cnt % 2) {
        used[v] = true;
        return true;
    }
    return false;
}

void dfs(std::vector<std::unordered_set<int>>& graph, std::vector<bool>& used, int v) {
    used[v] = true;

    for (auto nv: graph[v]) {
        if (!used[nv]) {
            dfs(graph, used, nv);
        }
    }
}

void solve() {
    int n, k, m;
    std::cin >> n >> k >> m;

    std::vector<std::unordered_set<int>> graph(n + k);

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;

        graph[start - 1].insert(end + n - 1);
    }

    std::vector<bool> used(n + k, false);

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            std::vector<bool> used_tmp(n + k, false);
            dfs(graph, used, used_tmp, i, 0);
            used_tmp.clear();
        }
    }
    
    std::vector<bool> new_used(n + k, false);
    
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(graph, new_used, i);
        }
    }
    
    int cnt_l = 0;
    for (int i = 0; i < n; ++i) {
        if (!new_used[i]) {
            ++cnt_l;
        }
    }
    std::cout << cnt_l << '\n';
    for (int i = 0; i < n; ++i) {
        if (!new_used[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << '\n';

    int cnt_r = 0;

    for (int i = n; i < n + k; ++i) {
        if (new_used[i]) {
            ++cnt_r;
        }
    }

    std::cout << cnt_r << '\n';

    for (int i = n; i < n + k; ++i) {
        if (new_used[i]) {
            std::cout << i + 1 - n << " ";
        }
    }
    std::cout << '\n';
}

// int main() {
//     solve();

//     return 0;
// }
