#include "index.h"

bool isPunct(const char& ch) {
    return std::ispunct(ch) || ch == ' ' || ch == '\n' || ch == '\t';
}

void getIndexFile(std::unordered_map<std::string, std::vector<indexFile>>& hash_words, std::vector<File>& files) {
    int id_file = files.size() - 1;
    int fdin = open(data(files[id_file].name), O_RDONLY);

    int number_row = 1;

    size_t size_file = std::filesystem::file_size(files[id_file].name);
    char* input_file = static_cast<char*>(mmap(0, size_file, PROT_READ, MAP_SHARED, fdin, 0));
    size_t dl = 0;

    char* cur_ch = input_file;
    char* start_word = cur_ch;
    bool word_starting = true;
    size_t len_word = 0;
    std::unordered_map<std::string, std::vector<int>> words;

    for (size_t i = 0; i < size_file; ++i, ++cur_ch, ++len_word) {
        if ((*cur_ch) == '\n') {
            ++number_row;
            len_word = 0;
        }
        if (word_starting && (isPunct(*cur_ch))) {
            std::string word{start_word, len_word};

            if (len_word) {
                words[word].push_back(number_row);
                ++dl;
            }
            word_starting = false;
            len_word = 0;
        }
        else if ((!word_starting) && (!isPunct(*cur_ch))) {
            start_word = cur_ch;
            word_starting = true;
            len_word = 0;
        }
    }
    if (word_starting) {
        std::string word{start_word, len_word};
        words[word].push_back(number_row);
        ++dl;
    }

    close(fdin);

    files[id_file].dl = dl;

    for (auto word : words) {
        hash_words[word.first].push_back({id_file, word.second});
    }
}

void putIndex(std::unordered_map<std::string, std::vector<indexFile>>& hash_words) {
    using namespace Index;

    flatbuffers::FlatBufferBuilder builder;

    std::vector<flatbuffers::Offset<Index::Node>> words_vector;

    std::vector<std::string> words;

    for (const auto& word : hash_words) {
        words.push_back(word.first);
    }

    std::sort(words.begin(), words.end());

    for (const auto& word : words) {
        std::vector<flatbuffers::Offset<Index::File>> files_vector;
        for (const auto& file : hash_words[word]) {
            std::vector<ulong> rows_vector;
            for (const auto& row : file.rows) {
                rows_vector.push_back(row);
            }
            files_vector.push_back(CreateFile(builder, file.id, builder.CreateVector(rows_vector)));
        }
        words_vector.push_back(CreateNode(builder, builder.CreateString(word), builder.CreateVector(files_vector)));
    }
    words.clear();
    hash_words.clear();

    builder.Finish(CreateFilesIndex(builder, builder.CreateVector(words_vector)));

    std::ofstream outfile_index("index.bin", std::ios::binary);

    outfile_index.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());
    outfile_index.close();
}

void putFilesId(std::vector<File>& files) {
    using namespace Files;

    uint64_t d_avg = 0;

    for (int i = 0; i < files.size(); ++i) {
        d_avg += files[i].dl;
    }

    d_avg /= files.size();

    flatbuffers::FlatBufferBuilder builder;

    std::vector<flatbuffers::Offset<name_file>> id_files_vector;

    for (int i = 0; i < files.size(); ++i) {
        auto sname = builder.CreateString(files[i].name);
        ulong id = i;
        ulong dl = files[i].dl;
        id_files_vector.push_back(Createname_file(builder, sname, id, dl));
    }
    builder.Finish(Createid_files(builder, builder.CreateVector(id_files_vector), d_avg));

    std::ofstream outfile_index("files_name.bin", std::ios::binary);

    outfile_index.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());
    outfile_index.close();
}

void indexing(std::string& path_directory) {
    std::unordered_map<std::string, std::vector<indexFile>> hash_words;
    std::vector<File> files;

    travelsalDirecotry(path_directory, files, hash_words);
    putIndex(hash_words);
    putFilesId(files);
}
