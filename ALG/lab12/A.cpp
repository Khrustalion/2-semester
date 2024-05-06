#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <algorithm>
#include <stdint.h>

void insert(std::vector<uint64_t>& hash, std::vector<bool>& free, uint64_t value) {
    uint64_t n = hash.size();
    uint64_t k = (value % (n * 10)) % n;

    while (!free[k]) {
        if (hash[k] == value) {
            break;
        }
        k = (k + 1) % n;
    }

    hash[k] = value;
    free[k] = false;
}

bool find(std::vector<uint64_t>& hash, std::vector<bool>& free, uint64_t value) {
    uint64_t n = hash.size();
    uint64_t k = (value % (n * 10)) % n;

    while (!free[k]) {
        if (hash[k] == value) {
            return true;
        }
        k = (k + 1) % n;
    }
    return false;
}

int solve() {
    uint64_t n;
    std::cin >> n;

    std::vector<std::pair<uint64_t, uint64_t>> input(n);

    for (uint64_t i = 0; i < n; ++i) {
        uint64_t x, y;
        std::cin >> x >> y;

        input[i] = {x, y};
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                bool first_line = false;
                bool second_line = false;
                bool third_line = false;

                bool find_ = true;

                std::vector<uint64_t> x_(2 * n, -1);
                std::vector<uint64_t> y_(2 * n, -1);

                std::vector<bool> free_x(2 * n, true);
                std::vector<bool> free_y(2 * n, true);

                for (int l = 0; l < n; ++l) {
                    int x = input[l].first;
                    int y = input[l].second;

                    if (find(x_, free_x, x) || find(y_, free_y, y)) {
                        continue;
                    }
                    if ((!first_line) && i == 0) {
                        insert(x_, free_x, x);
                        first_line = true;
                    }
                    else if ((!first_line)) {
                        insert(y_, free_y, y);
                        first_line = true;
                    }
                    else if ((!second_line) && j == 0) {
                        insert(x_, free_x, x);
                        second_line = true;
                    }
                    else if (!second_line) {
                        insert(y_, free_y, y);
                        second_line = true;
                    }
                    else if ((!third_line) && k == 0) {
                        insert(x_, free_x, x);
                        third_line = true;
                    }
                    else if (!third_line) {
                        insert(y_, free_y, y);
                        third_line = true;
                    }
                    else {
                        find_ = false;
                        break;
                    }
                }
                if (find_ == true) {
                    std::cout << "YES";
                    return 1;
                }
            }
        }
    }
    std::cout << "NO";
    return 1;
}

// int main() {
//     solve();
//     return 0;
// }
