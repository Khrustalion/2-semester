#include <iostream>
//#include <blazingio.hpp>
#include <string>
#include <vector>

void prefixFunction(std::string& s, std::vector<int>& prefix) {
    size_t n = s.size();
    prefix[0] = 0;
    for (int i = 1; i < n; ++i) {
        int k = prefix[i-1];
        while (k > 0 && s[i] != s[k]) {
            k = prefix[k-1];
        }
        if (s[i] == s[k]) {
            ++k;
        }
        prefix[i] = k;
    }   
}

void solve() {
    std::string s;
    std::cin >> s;

    size_t len_x = s.size();

    std::vector<int> prefix(len_x, 0);

    prefixFunction(s, prefix);

    for (int i = 0; i < len_x; ++i) {
        std::cout << prefix[i] << " ";
    }
}

int main() {
    solve();
    return 0;
}