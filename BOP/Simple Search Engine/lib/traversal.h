#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "index.h"

struct indexFile;

struct File {
    std::string name;
    int dl;
};


void travelsalDirecotry(const std::string& current_path, std::vector<File>& files, std::unordered_map<std::string, std::vector<indexFile>>& hash_words);
