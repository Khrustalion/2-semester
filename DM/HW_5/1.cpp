#include <iostream>
#include <set>
#include <vector>

void floyd(std::set<int>*& graph, int**& dist, int n) {
    for (int i = 0; i < n; ++i) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (dist[u][i] != -1 && dist[i][v] != -1 && (dist[u][v] == -1 || dist[u][v] > dist[u][i] + dist[i][v])) {
                    dist[u][v] = dist[u][i] + dist[i][v];
                }
            }
        }
    }
}

void task_1() {
    int n, m;
    std::cin >> n >> m;

    std::set<int>* graph = new std::set<int>[n];

    int** dist = new int*[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            dist[i][j] = -1;
        }
        dist[i][i] = 0;
    }

    for (int i = 0; i < m; ++i) {
        int start, end;
        std::cin >> start >> end;

        graph[start - 1].insert(end - 1);
        graph[end - 1].insert(start - 1);

        dist[start - 1][end - 1] = 1;
        dist[end - 1][start - 1] = 1;
    }

    floyd(graph, dist, n);

    int rad = -1;
    int diam = 0;

    std::vector<int> centers;

    for (int i = 0; i < n; ++i) {
        int e = 0;

        for (int j = 0; j < n; ++j) {
            e = std::max(e, dist[i][j]);
        }
        rad = std::min(rad == -1 ? e : rad, e);
        diam = std::max(diam, e);
        std::cout << "e(" << char('a' + i) << "): " << e << '\n';
    }

    for (int i = 0; i < n; ++i) {
        int e = 0;

        for (int j = 0; j < n; ++j) {
            e = std::max(e, dist[i][j]);
        }

        if (e == rad) {
            centers.push_back(i);
        }
    }

    std::cout << "rad: " << rad << '\n';
    std::cout << "diam: " << diam << '\n';

    std::cout << "centers: ";

    for (int i = 0; i < centers.size(); ++i) {
        std::cout << char('a' + centers[i]) << ", ";
    }
    std::cout << '\n';
}