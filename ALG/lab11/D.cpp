#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <queue>
#include <algorithm>

void solve() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> cats(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> cats[i][j];
        }
    }

    std::vector<std::vector<int>> owners(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> owners[i][j];
        }
    }

    std::vector<std::pair<int, int>> answer(n, {-1, -1});

    std::queue<int> cats_q;
    for (int i = 0; i < n; ++i) {
        cats_q.push(i);
    }

    while (!cats_q.empty()) {
        int cat = cats_q.front();
        cats_q.pop();

        int owner;

        for (int i = 0; i < n; ++i) {
            if (cats[cat][i] != -1) {
                owner = cats[cat][i];
                cats[cat][i] = -1;
                break;
            }
        }

        if (answer[owner].first == -1) {
            answer[owner].first = cat;
            answer[owner].second = owner;
        }
        else {
            int desire;

            for (int i = 0; i < n; ++i) {
                if (owners[owner][i] == cat || owners[owner][i] == answer[owner].first) {
                    desire = owners[owner][i];
                    break;
                }
            }

            if (desire == answer[owner].first) {
                cats_q.push(cat);
            } else {
                cats_q.push(answer[owner].first);
                answer[owner].first = cat;
            }
        }
    }
    std::sort(answer.begin(), answer.end());

    for (int i = 0; i < n; ++i) {
        std::cout << answer[i].first << " " << answer[i].second << '\n';
    }
}

// int main() {
//     solve();
//     return 0;
// }