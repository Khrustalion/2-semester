#include <lib/traversal.cpp>
#include <lib/ranking.cpp>

struct parseArg {
    std::string path_directory;
    std::string path_request;

    size_t amount_files;

    bool indexing = false;
    bool ranking = false;
};

void parse(int argc, char** argv, parseArg& parse_arg) {
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--path_directory")) {
            parse_arg.path_directory = argv[++i];
        } else if (!std::strcmp(argv[i], "--path_request")) {
            parse_arg.path_request = argv[++i];
        } else if (!std::strcmp(argv[i], "--indexing")) {
            parse_arg.indexing = true;
        } else if (!std::strcmp(argv[i], "--ranking")) {
            parse_arg.ranking = true;
        } else if (!std::strcmp(argv[i], "--amount_files")) {
            parse_arg.amount_files = std::stoll(argv[++i]);
        }
    }
}

int main(int argc, char** argv) {
    parseArg parse_arg;

    parse(argc, argv, parse_arg);

    if (parse_arg.indexing) {
        indexing(parse_arg.path_directory);
    }
    if (parse_arg.ranking) {
        Ranking rank;

        rank.ranking(parse_arg.amount_files);
    }    
    return 0;
}