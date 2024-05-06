#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <stdint.h>

uint64_t getHash1(std::string s) {
    uint64_t hash = 0;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        hash += (s[i] - 'a' + 1) * std::pow(2, i);
    }
    return hash;
} 

uint64_t getHash2(std::string s) {
    uint64_t hash = 0;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        hash += (s[i] - 'a' + 1) * std::pow(7, i);
    }
    return hash;
}

class hash_table {
    public:
    std::vector<std::string> table_1;
    std::vector<std::string> table_2;
    std::vector<bool> free_1;
    std::vector<bool> free_2;
    int n;

    hash_table() = default;

    hash_table(int n_) {
        n = n_;
        table_1 = std::vector<std::string>(n);
        table_2 = std::vector<std::string>(n);
        free_1 = std::vector<bool>(n, true);
        free_2 = std::vector<bool>(n, true);
    }

    void insert(std::string s) {
        uint64_t hash1 = getHash1(s) % n;
        uint64_t hash2 = getHash2(s) % n;
        if (free_1[hash1]) {
            table_1[hash1] = s;
            free_1[hash1] = false;
        }
        else if (free_2[hash2]) {
            table_2[hash2] = s;
            free_2[hash2] = false;
        } else {
            std::string tmp = table_1[hash1];
            table_1[hash1] = s;
            this->insert(tmp);
        }   
    }

    bool find(std::string s) {
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

void find(std::vector<std::string>& v, std::string s) {
    for (auto x : v) {
        if (x == s) {
            std::exit(1);
        }
    }
}

void solve() {
    int n;
    std::cin >> n;

    hash_table p1(4 * n);
    hash_table p2(4 * n);
    hash_table p3(4 * n);

    std::vector<std::string> f1;
    std::vector<std::string> f2;
    std::vector<std::string> f3;

    std::string file;

    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        p1.insert(file);
        f1.push_back(file);
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        p2.insert(file);
        f2.push_back(file);
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> file;

        p3.insert(file);
        f3.push_back(file);
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