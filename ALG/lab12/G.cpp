#include <iostream>
//#include <blazingio.hpp>
#include <set>
#include <vector>
#include <cmath>
#include <algorithm>

long long getHash(std::vector<long long>& v) {
    long long hash = 0;
    for (long long i = 0; i < v.size(); ++i) {
        hash += (v[i] * std::pow(2, i));
    }
    return hash;
}


void solve() {
    long long n;
    std::cin >> n;

    std::vector<long long> degrees_1(n + 1, 0);
    std::vector<long long> degrees_2(n + 1, 0);

    for (long long i = 0; i < n-1; ++i) {
        long long start, end;
        std::cin >> start >> end;

        ++degrees_1[start];
        ++degrees_1[end];
    }
    for (long long i = 0; i < n-1; ++i) {
        long long start, end;
        std::cin >> start >> end;

        ++degrees_2[start];
        ++degrees_2[end];
    }

    std::sort(degrees_1.begin(), degrees_1.end());
    std::sort(degrees_2.begin(), degrees_2.end());

    if (getHash(degrees_1) == getHash(degrees_2) && degrees_1 == degrees_2) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
}

// int main() {
//     solve();
//     return 0;
// }