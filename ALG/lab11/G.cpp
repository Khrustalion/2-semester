#include <iostream>
//#include <blazingio.hpp>
#include <set>
#include <vector>

bool find(std::vector<int>& vec, int v) {
    for (auto x: vec) {
        if (v == x) {
            return true;
        }
    }
    return false;
}

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> q(2 * n, -1);
    int q_start = 0, q_end = -1;

    std::vector<std::set<int>> graph(n);

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;
        graph[start].insert(end);
        graph[end].insert(start);

        if (!find(q, start)) {
            q[++q_end] = start;
        }
        if (!find(q, end)) {
            q[++q_end] = end;
        }
    }

    for (int _ = 0; _ < n; ++_) {
        if (graph[q[q_start]].find(q[q_start + 1]) != graph[q[q_start]].end()) {
            q[++q_end] = q[q_start];
            ++q_start;
        }
        else {
            int index;
            for (int i = q_start; i <= q_end; ++i) {
                if (graph[i].find(q[q_start]) != graph[i].end() && graph[i + 1].find(q[q_start + 1]) != graph[i + 1].end()) {
                    index = i;
                    break;
                }
            }

            for (int i = 0; i <= (index - q_start) / 2; ++i) {
                std::swap(q[q_start + i + 1], q[index - i]);
            }
        }
    }
    for (int i = q_start; i <= q_end; ++i) {
        std::cout << q[i] << " ";
    }
}

// int main() {
//     solve():
//     return 0;
// }