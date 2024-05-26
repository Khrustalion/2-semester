#include <iostream>
//#include <blazingio.hpp>
#include <string>
#include <vector>
#include <stdint.h>
#include <cmath>

#define k_ 2

void getPrefixHash(std::string& s, std::vector<uint64_t>& prefix) {
    int n = prefix.size();
    prefix[0] = 0;
    for (int i = 1; i < n; ++i) {
        prefix[i] = prefix[i-1] + s[i-1] * std::pow(k_, i - 1);
    }
}

void solve() {
    int n, m;
    std::cin >> n >> m;

    std::string text;
    std::cin >> text;

    std::vector<uint64_t> prefix(n+1, 0);

    getPrefixHash(text, prefix);

    int answer = 0;

    // for (int i = 0; i < n+1; ++i) {
    //     std::cout << prefix[i] << " ";
    // }

    // std::cout << '\n';

    for (int _ = 0; _ < m; ++_) {
        int i, j, k;
        std::cin >> i >> j >> k;

        int m = std::max(i + k, j + k);

        uint64_t hash_1 = (prefix[j + k - 1] - prefix[j-1]) * std::pow(k_, m - j);
        uint64_t hash_2 = (prefix[i + k - 1] - prefix[i-1]) * std::pow(k_, m - i);

        

        if (hash_1 == hash_2 && text.substr(j-1, k) == text.substr(i-1, k)) {
            ++answer;
        }
    }
    std::cout << answer;
}

int main() {
    solve();
    return 0;
}