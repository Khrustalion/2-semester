#include "lib/Scheduler.cpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

struct stryc {
    int a = 0;
};


int main() {
    Scheduler scheduler;


    auto id1 = scheduler.add([] (std::vector<int> s, int n) {
        s.push_back(n);
        return s;},
        std::vector<int>({1, 3}), 2);

    auto id2 = scheduler.add([] (int a, int b) {
        std::vector<int> s({a, b});

        return s;
        },4, 5);
    
    auto id3 = scheduler.add([] (std::vector<int> a, std::vector<int> b) {
        for (auto x : b) {
            a.push_back(x);
        }
        return a;
    }, scheduler.getFutureResult<std::vector<int>>(id1), scheduler.getFutureResult<std::vector<int>>(id2));

    auto answer = scheduler.getResult<std::vector<int>>(id3);

    for (auto x : answer) {
        std::cout << x <<  " ";
    }

    std::cout << '\n';
    return 0;
}