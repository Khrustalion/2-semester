#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <string_view>
#include <sys/mman.h>
#include <iostream>
#include <filesystem>
#include <fcntl.h>
#include "../files_name_generated.h"
#include "../index_generated.h"
#include "traversal.h"
#include <algorithm>
#include <stack>

struct File;

struct indexFile {
    int id;
    std::vector<int> rows;

    indexFile(int id_, const std::vector<int>& rows_) : id{id_}, rows{rows_} {};
};

bool isPunct(const char& ch);

void getIndexFile(std::unordered_map<std::string, std::vector<indexFile>>& hash_words, std::vector<File>& files);

void putIndex(std::unordered_map<std::string, std::vector<indexFile>>& hash_words);

void putFilesId(std::vector<File>& files);

int getWordFiles(const std::string_view& word, std::vector<int>& files);

void indexing(std::string& path_directory);
