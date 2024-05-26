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
    std::string T;
    std::string X;
    std::cin >> T >> X;
    std::string s = X + '#' + T;

    size_t len_x = X.size();
    size_t n = T.size() + len_x + 1;

    std::vector<int> prefix(n, 0);

    prefixFunction(s, prefix);


    int answer = 0;
    for (int i = 1; i < n; ++i) {
        if (prefix[i] == len_x) {
            ++answer;
        }
    }
    std::cout << answer << '\n';
    for (int i = 1; i < n; ++i) {
        if (prefix[i] == len_x) {
            std::cout << i - 2*len_x << " ";
        }
    }
}

int main() {
    solve();
    return 0;
}