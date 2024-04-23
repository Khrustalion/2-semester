#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <set>

bool dfs(std::vector<std::set<int>>& graph, std::vector<bool>& used, std::vector<int>& matching, int v) {
    if (used[v]) {
        return false;
    }

    used[v] = true;

    for (auto nv : graph[v]) {
        if (matching[nv] == -1  || dfs(graph, used, matching, matching[nv])) {
            matching[nv] = v;
            return true;
        }
    }
    return false;
}

void solve() {
    int n, m, q;
    std::cin >> n >> m >> q;

    int k = n * m;
    std::vector<std::set<int>> graph(k);
    for (int i = 0; i < k; ++i) {
        if ((i / m) == ((i + 1) / m)) {
            graph[i].insert(i + 1);
        }
        if ((i / m) == ((i - 1) / m)) {
            graph[i].insert(i - 1);
        }
        if (i - m >= 0) {
            graph[i].insert(i - m);
        }
        if (i + m < k) {
            graph[i].insert(i + m);
        }
    }

    for (int i = 0; i < q; ++i) {
        int start, end;
        std::cin >> start >> end;

        int number = (start - 1) * m + end - 1;

        for (int j = 0; j < k; ++j) {
            graph[number].extract(j);
        }
        if (number < k - 1) {
            graph[number + 1].extract(number);
        }
        if (number > 0) {
            graph[number - 1].extract(number);
        }
        if (number - m >= 0) {
            graph[number - m].extract(number);
        }
        if (number + m < k) {
            graph[number + m].extract(number);
        }
    }

    int answer = 0;

    std::vector<int> matching(k, -1);
    for (int i = 0; i < k; ++i) {
        std::vector<bool> used(k, false);
        if (dfs(graph, used, matching, i)) {
            ++answer;
        }
        used.clear();
    }

    // for (int i = 0; i < k; ++i) {
    //     if (matching[i] != -1) {
    //         std::cout << i << " mathcing with: " << matching[i] << '\n';
    //     }
    // }

    std::cout << answer;
}

// int main() {
//     solve();
//     return 0;
// }