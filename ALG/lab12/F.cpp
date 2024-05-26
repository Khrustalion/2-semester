#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <stdint.h>

uint32_t getHash(std::string str) {
    uint32_t h = 0;
    for (auto c : str) {
        h ^= c;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
}

uint64_t getHash1(uint32_t x) {
    return (x * 31 + 157) % (int)1e5;
} 

uint64_t getHash2(uint32_t x) {
    return ((x * 211 + 13) % ((int)1e5 + 2)) % (int)1e5;
}

class hash_table {
    public:
    std::vector<uint32_t> table_1;
    std::vector<uint32_t> table_2;
    std::vector<bool> free_1;
    std::vector<bool> free_2;
    int n;

    hash_table() = default;

    hash_table(int n_) {
        n = n_;
        table_1 = std::vector<uint32_t>(n);
        table_2 = std::vector<uint32_t>(n);
        free_1 = std::vector<bool>(n, true);
        free_2 = std::vector<bool>(n, true);
    }

    void insert(uint32_t h) {
        uint64_t hash1 = getHash1(h) % n;
        uint64_t hash2 = getHash2(h) % n;
        if (free_1[hash1]) {
            table_1[hash1] = h;
            free_1[hash1] = false;
        }
        else if (free_2[hash2]) {
            table_2[hash2] = h;
            free_2[hash2] = false;
        } else {
            uint32_t tmp = table_2[hash2];
            table_2[hash2] = h;
            this->insert(tmp);
        }   
    }

    bool find(uint32_t s) {
        uint64_t hash1 = getHash1(s) % n;
        if ((!free_1[hash1]) && (s == table_1[hash1])) {
            return true;
        }
        uint64_t hash2 = getHash2(s) % n;
        if ((!free_2[hash2]) && (s == table_2[hash2])) {
            return true;
        }
        return false;
    }
};

void solve() {
    int n;
    std::cin >> n;

    hash_table p1(8 * n);
    hash_table p2(8 * n);
    hash_table p3(8 * n);

    std::vector<uint32_t> f1;
    std::vector<uint32_t> f2;
    std::vector<uint32_t> f3;

    std::string file;

    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        uint32_t h = getHash(file);

        p1.insert(h);
        f1.push_back(h);
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        uint32_t h = getHash(file);

        p2.insert(h);
        f2.push_back(h);
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        uint32_t h = getHash(file);

        p3.insert(h);
        f3.push_back(h);
    }


    int b1 = 0;
    int b2 = 0;
    int b3 = 0;

    for (int i = 0; i < n; ++i) {
        bool find2 = p2.find(f1[i]);
        bool find3 = p3.find(f1[i]);

        if ((!find2) && (!find3)) {
            b1 += 3;
        }
        else if (!(find2 && find3)) {
            b1 += 1;
        }
    }
    for (int i = 0; i < n; ++i) {
        bool find1 = p1.find(f2[i]);
        bool find3 = p3.find(f2[i]);

        if ((!find1) && (!find3)) {
            b2 += 3;
        }
        else if (!(find1 && find3)) {
            b2 += 1;
        }
    }
    for (int i = 0; i < n; ++i) {
        bool find1 = p1.find(f3[i]);
        bool find2 = p2.find(f3[i]);

        if ((!find1) && (!find2)) {
            b3 += 3;
        }
        else if (!(find1 && find2)) {
            b3 += 1;
        }
    }

    std::cout << b1 << " " << b2 << " " << b3;
}

// int main() {
//     solve();
//     return 0;
// }