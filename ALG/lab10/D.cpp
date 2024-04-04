#include <iostream>
#include <set>
#include <vector>
#include <stack>

void dfs(std::set<std::pair<int, int>>* graph, int v, std::vector<bool>& visited) {
    visited[v] = true;

    for (auto nv: graph[v]) {
        if (!visited[nv.first]) {
            dfs(graph, nv.first, visited);
        }   
    }
}

bool checkCircle(std::set<std::pair<int, int>>* graph, int n) {
    for (int i = 0; i < n; ++i) {
        if (graph[i].size() % 2) {
            return false;
        }
    }

    std::vector<bool> visited(n, false);

    dfs(graph, 0, visited);


    for (int i = 0; i < n; ++i) {
        if (graph[i].size() && !visited[i]) {
            return false;
        }
    }
    return true;
}

void solve() {
    int n, m;
    std::cin >> n >> m;
    
    if (m == 0) {
        std::exit(1);
    }

    std::set<std::pair<int, int>>* graph = new std::set<std::pair<int, int>>[n];

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;
        if (start == end) {
            std::exit(1);
        }
        graph[start - 1].insert({end - 1, i});
        graph[end - 1].insert({start - 1, i});
    }

    if (!checkCircle(graph, n)) {
        std::cout << ":(";
    } else {
        std::stack<int> s;
        s.push(0);
        std::vector<int> answer;

        while (s.size()) {
            int v = s.top();
        
            if (!graph[v].size()) {
                s.pop();
                answer.push_back(v + 1);
            }
            else {
                int min_time = -1;
                std::pair<int, int> min_nv;
                for (auto nv: graph[v]) {
                    if (min_time == -1 || nv.second < min_time) {
                        min_time = nv.second;
                        min_nv = nv;
                    }
                }
                s.push(min_nv.first);

                graph[v].extract(min_nv);
                graph[min_nv.first].extract({v, min_nv.second});
            }
        }
        for (int i = m; i >= 0; --i) {
            std::cout << answer[i] << " ";
        }
    }
}

// int main() {
//     solve();
//     return 0;
// }
