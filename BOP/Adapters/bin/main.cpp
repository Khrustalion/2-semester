#include <lib/Adapters.h>
#include <vector>
#include <map>
#include <list>
#include <type_traits>


bool foo(int i) {
    return i % 2;
}

int f(int i) {
    return i * i - 1;
}

int main() {
    std::map<int, int> l;
    l.insert({1, 0});
    l.insert({2, 1});
    l.insert({3, 2});

    //auto adap = l | transform([] (int i) {return i*2;}) | transform([] (int i) {return i*3;});

    auto adap = l | values();
    
    for (auto a : adap) {
        std::cout << a << " ";
    }
    return 0;
}