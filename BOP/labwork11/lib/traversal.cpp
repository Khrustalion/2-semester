#include "traversal.h"

void travelsalDirecotry(const std::string& current_path, std::vector<File>& files, std::unordered_map<std::string, std::vector<indexFile>>& hash_words) {
    if (!std::filesystem::is_directory(current_path)) {
        files.push_back({current_path, 0});
        getIndexFile(hash_words, files);
    } else {
        for (const auto& file : std::filesystem::directory_iterator(current_path)) {
            std::string file_string = file.path().string();
            if (std::filesystem::is_directory(file)) {
                travelsalDirecotry(file_string, files, hash_words);
                continue;
            }
            files.push_back({file_string, 0});
            getIndexFile(hash_words, files);
        }
    }
}