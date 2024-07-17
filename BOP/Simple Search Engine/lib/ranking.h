#include "index.h"
#include <cmath>
#include <queue>
#include <set>
#include <fcntl.h>

const int K = 2;
const int B = 1;

enum Operation {
    AND,
    OR, 
    WORD,
    RPAREN,
    LPAREN
};


struct Node {
    ~Node() {
        files.clear();
    }
    
    std::vector<int> files;
    Operation oper = Operation::OR;
};

class Ranking {
public:
    ~Ranking();

    Ranking();

    void getOperationResult(const std::vector<int>& files1, const std::vector<int>& files2, std::vector<int>& result, Operation oper);

    int getWordFiles(const std::string_view& word, std::vector<int>& files);

    void pushInStack(std::stack<Node>& s, const std::string_view& word_view);

    void getOperations(const std::string& path, std::vector<std::string>& words, std::vector<Operation>& oper);

    void getRequestFiles(const std::string& file, std::vector<int>& files, std::vector<std::string>& words);

    char* getDataIndex();

    char* getDataFilesName();

    double getTFIWord(const std::string& word, int file_id, int d_avg, int N, int dl);

    int getTFI(const std::vector<std::string>& words, int file, int d_avg, int N, int dl);

    std::vector<int> getBestFiles(const std::vector<std::string>& words, const std::vector<int>& files_vec, size_t amount_files);

    std::string getFileName(int file);

    void getRows(const std::vector<std::string>& words, int file, std::set<int>& rows);

    void ranking(size_t amount_files);

    void printRows(std::vector<std::string>& words, std::vector<int>& best_files);

private:
    char* data_index;
    char* data_files_name;

    int fdin_index;
    int fdin_files_name;
};


