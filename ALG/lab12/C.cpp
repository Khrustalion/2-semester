#include <iostream>
//#include <blazingio.hpp>
#include <vector>

void insert(std::vector<long long>& hash, std::vector<bool>& free, long long value) {
    long long n = hash.size();
    long long k = (value % (n * 2)) % n;

    while (!free[k]) {
        if (hash[k] == value) {
            break;
        }
        k = (k + 1) % n;
    }

    hash[k] = value;
    free[k] = false;
}

bool contain(std::vector<long long>& hash, std::vector<bool>& free, long long value) {
    long long n = hash.size();
    long long k = (value % (n * 2)) % n;

    long long cnt = 0;

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
    long long n;
    std::cin >> n;

    std::vector<long long> hash(n*2, 0);
    std::vector<bool> free(n*2, true);

    for (long long i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;
        insert(hash, free, value);
    }

    bool answer = true;

    for (long long i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;

        if (!contain(hash,  free, value)) {
            answer = false;
            break;

        }
    }

    if (!answer) {
        std::cout << "NO";
    } else {
        std::cout << "YES";
    }
}

// int main() {
//     solve();
//     return 0;
// }