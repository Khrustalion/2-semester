#include <iostream>
//#include <blazingio.hpp>
#include <vector>
#include <set>
#include <stdint.h>

struct hash_table {
    std::vector<uint64_t> hash;
    std::vector<uint64_t> keys;
    std::vector<std::vector<uint64_t>> pouint64_ts;
    std::vector<bool> free;
    std::vector<bool> need_check;
    uint64_t sum = 0;
    uint64_t max_ = 0;
    uint64_t cnt = 0;
    uint64_t cnt_student = 0;

    hash_table() = default;

    hash_table(uint64_t n) {
        hash = std::vector<uint64_t>(n);
        free = std::vector<bool>(n, true);
        need_check = std::vector<bool>(n, false);
        pouint64_ts = std::vector<std::vector<uint64_t>>(n);
    }

    uint64_t Max() {
        return max_;
    }

    uint64_t avg() {
        return (cnt != 0) ? sum / cnt : 0;
    }

    void reHash() {
        uint64_t n = hash.size();
        uint64_t new_n = n * 2;

        sum = 0;
        max_ = 0;
        cnt = 0;
        cnt_student = 0;

        std::vector<uint64_t> last_hash = hash;
        std::vector<std::vector<uint64_t>> last_pouint64_ts = pouint64_ts;

        hash = std::vector<uint64_t>(new_n);
        free = std::vector<bool>(new_n, true);
        pouint64_ts = std::vector<std::vector<uint64_t>>(new_n);
        need_check = std::vector<bool>(new_n, false);

        for (uint64_t i = 0; i < n; ++i) {
            for (auto pouint64_t : last_pouint64_ts[i]) {
                this->insert(last_hash[i], pouint64_t);
            }
        }

        last_hash.clear();
        last_pouint64_ts.clear();
    }

    void del(uint64_t isu) {
        uint64_t n = hash.size();
        uint64_t k = this->find(isu);

        //std::cout << k << " " << hash[k] << '\n';

        if (hash[k] == isu) {
            free[k] = true;

            max_ = 0;

            for (auto key : keys) {
                if (key != k && !free[key]) {
                    for (auto pouint64_t : pouint64_ts[key]) {
                        if (pouint64_t > max_) {
                            max_ = pouint64_t;
                        }
                    }
                }
            }

            uint64_t diff = 0;
            for (auto pouint64_t : pouint64_ts[k]) {
                diff += pouint64_t;
            }

            sum -= diff;
            cnt -= pouint64_ts[k].size();
            cnt_student -= 1;

            pouint64_ts[k].clear();
        }
    }

    void insert(uint64_t isu, uint64_t pouint64_t) {
        uint64_t n = hash.size();
        uint64_t k = this->find(isu);

        if (free[k] == true) {
            ++cnt_student;
        }

        keys.push_back(k);

        hash[k] = isu;
        pouint64_ts[k].push_back(pouint64_t);
        free[k] = false;
        need_check[k] = true;

        if (pouint64_t > max_) {
            max_ = pouint64_t;
        }

        sum += pouint64_t;

        ++cnt;

        if (cnt_student == hash.size()) {
            this->reHash();
        }
    }

    uint64_t find(uint64_t isu) {
        uint64_t n = hash.size();
        uint64_t k = (isu % (n * 10)) % n;

        while (need_check[k]) {
            if (hash[k] == isu) {
                return k;
            }
            k = (k + 1) % n;
        }
        return k;
    }
};

void solve() {
    uint64_t m, q;
    std::cin >> m >> q;

    std::vector<hash_table*> hash(m);

    for (uint64_t i = 0; i < m; ++i) {
        hash[i] = new hash_table(4);
    }

    for (uint64_t i = 0; i < q; ++i) {
        char oper;

        std::cin >> oper;

        if (oper == '-') {
            uint64_t group, isu;
            std::cin >> group >> isu;

            group -= 1;

            hash[group]->del(isu);
        }

        else if (oper == '+') {
            uint64_t group, isu, pouint64_t;
            std::cin >> group >> isu >> pouint64_t;

            group -= 1;

            hash[group]->insert(isu, pouint64_t);
        }
        else if (oper == 'a') {
            uint64_t group;
            std::cin >> group;

            group -= 1;

            std::cout << hash[group]->avg() << '\n';
        }
        else if (oper == 'm') {
            uint64_t group;
            std::cin >> group;

            group -= 1;

            std::cout << hash[group]->Max() << '\n';
        }
    }
}

// int main() {
//     solve();
//     return 0;
// }