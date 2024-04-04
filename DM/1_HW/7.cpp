#include <iostream>
#include <set>

void task_7() {
    int n, m;
    std::cin >> n >> m;

    std::set<int>* graph = new std::set<int>[n];

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;

        graph[start].insert(end);
        graph[end].insert(start);
    }

    
}   