#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <stdint.h>

void insert(std::vector<uint64_t>& hash, std::vector<bool>& free, std::vector<uint64_t>& answer, uint64_t value) {
    uint64_t n = hash.size();
    uint64_t k = (value % (n * 10)) % n;

    while (!free[k]) {
        if (hash[k] == value) {
            ++answer[k];
            break;
        }
        k = (k + 1) % n;
    }

    hash[k] = value;
    free[k] = false;
}

bool contain(std::vector<uint64_t>& hash, std::vector<bool>& free, uint64_t value) {
    uint64_t n = hash.size();
    uint64_t k = (value % (n * 10)) % n;

    uint64_t cnt = 0;

    while (!free[k] && cnt != n) {
        if (hash[k] == value) {
            return true;
        }
        k = (k + 1) % n;
        ++cnt;
    }

    return false;
}

void solve() {
    uint64_t n;
    std::cin >> n;

    std::vector<uint64_t> hash(n, 0);
    std::vector<bool> free(n, true);
    std::vector<uint64_t> answer(n, 1);

    for (uint64_t i = 0; i < n; ++i) {
        uint64_t value;
        std::cin >> value;
        insert(hash, free, answer, value - i);
    }

    uint64_t answer_ = 0;

    for (auto v : answer) {
        answer_ += (v * (v - 1)) / 2;
    }

    std::cout << answer_;
}

// int main() {
//     solve();
//     return 0;
// }