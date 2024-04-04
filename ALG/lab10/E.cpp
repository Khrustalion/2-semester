#include <iostream>
#include <set>
#include <vector>

void dfs(std::set<int>*& graph, int*& tin, int*& tout, bool* visited, std::set<std::pair<int, int>>& answer, int parent, int time, int v) {
	++time;
	tin[v] = time;
	tout[v] = time;

	visited[v] = true;

	for (auto nv : graph[v]) {
		if (nv == parent) {
			continue;
		}
		if (visited[nv]) {
			tout[v] = std::min(tout[v], tin[nv]);
		}
		if (!visited[nv]) {
			dfs(graph, tin, tout, visited, answer, v, time, nv);
			tout[v] = std::min(tout[v], tout[nv]);

			if (tout[nv] == tin[nv] && tin[v] == tin[v]) {
				answer.insert({ std::min(v, nv), std::max(v, nv) });
			}
		}
	}
}

void dfs(std::set<int>*& graph, int color, std::vector<int>& colors, int v, std::set<std::pair<int, int>> bridges) {
    colors[v] = color;

    for (auto nv: graph[v]) {
        //std::cout << nv << '\n';
        if (colors[nv] == -1 && bridges.find({std::min(v, nv), std::max(v, nv)}) == bridges.end()) {
            dfs(graph, color, colors, nv, bridges);
        }
    }
}

void solve() {
	int n, m, q;
	std::cin >> n >> m >> q;

    ++n;

	std::set<int>* graph = new std::set<int>[n];

	for (int i = 0; i < m; ++i) {
		int start, end;
		std::cin >> start >> end;

		graph[start].insert(end);
		graph[end].insert(start);
	}

	bool* visited = new bool[n];
	int* tin = new int[n];
	int* tout = new int[n];
	std::set<std::pair<int, int>> bridges;


	for (int i = 0; i < n; ++i) {
		visited[i] = false;

	}

	for (int i = 0; i < n; ++i) {
		if (!visited[i]) {
			dfs(graph, tin, tout, visited, bridges, -1, -1, i);
		}
	}

	std::vector<int> colors(n, -1);
    int color = 0;
    for (int i = 0; i < n; ++i) {
        if (colors[i] == -1) {
            dfs(graph, color, colors, i, bridges);
            ++color;
        }
    }

    for (int i = 0; i < q; ++i) {
        int start, end;
        std::cin >> start >> end;

        if (colors[start] != colors[end]) {
            std::cout << "NO\n";
        }
        else {
            std::cout << "YES\n";
        }
    }
}

// int main() {
//     solve();
//     return 0;
// }