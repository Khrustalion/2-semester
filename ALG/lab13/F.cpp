#include <iostream>
//#include <blazingio.hpp>
#include <regex>
#include <string>



void solve() {
    int m;
    std::cin >> m;

    std::regex r(R"(([A-Z]([a-z])*\s){3}[0-9]{2}\s(\+7|7|8)([0-9]{3}|\([0-9]{3}\))-?[0-9]{3}(-?[0-9]{2}){2}\sg\.[A-Z]{3},\sul\.[A-Z]([a-z])*,\sd\.[0-9][0-9]+)");

    std::string text;

    std::getline(std::cin, text);

    for (int i = 0; i < m; ++i) {        
        std::getline(std::cin, text);

        if (std::regex_match(text, r)) {
            std::cout << "YES\n";
        }
        else {
            std::cout << "NO\n";
        }
    } 
    
}

int main() {
    solve();
    return 0;
}