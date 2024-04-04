#include <iostream>
#include <set>
#include <vector>

void solve() {
    int n, m, k;
    std::cin >> n >> m >> k;

    std::set<std::vector<int>>* graph = new std::set<std::vector<int>>[n];
    for (int i = 0; i < m; ++i) {
        int s, t;
        std::cin >> s >> t;

        std::vector<int> islands(s);

        for (int i = 0; i < s; ++i) {
            std::cin >> islands[i];
        }

        for
    }
}

// int main() {
//     solve();
//     return 0;
// }