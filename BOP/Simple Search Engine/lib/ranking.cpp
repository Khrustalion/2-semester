#include "ranking.h"

Ranking::~Ranking() {
    close(fdin_index);
    close(fdin_files_name);
}

Ranking::Ranking() {
    fdin_index = open("index.bin", O_RDONLY);

    size_t size_file_index = std::filesystem::file_size("index.bin");
    data_index = static_cast<char*>(mmap(0, size_file_index, PROT_READ, MAP_SHARED, fdin_index, 0));

    fdin_files_name = open("files_name.bin", O_RDONLY);

    size_t size_file_files_name = std::filesystem::file_size("files_name.bin");
    data_files_name = static_cast<char*>(mmap(0, size_file_files_name, PROT_READ, MAP_SHARED, fdin_files_name, 0));
}   

void Ranking::getOperationResult(const std::vector<int>& files1, const std::vector<int>& files2, std::vector<int>& result, Operation oper) {
    int v1 = 0, v2 = 0;
    while ((v1 < files1.size()) && (v2 < files2.size())) {
        if (files1[v1] == files2[v2]) {
            result.push_back(files2[v2]);
            ++v1, ++v2;
        } else if (files1[v1] < files2[v2] && oper == Operation::AND) ++v1;
        else if (files1[v1] < files2[v2] && oper == Operation::OR) result.push_back(files1[v1++]);
        else if (oper == Operation::AND) ++v2;
        else result.push_back(files2[v2++]);
    }
    while ((v1 < files1.size()) && oper == Operation::OR) {
        result.push_back(files1[v1++]); 
    }
    while ((v2 < files2.size()) && oper == Operation::OR) {
        result.push_back(files2[v2++]); 
    }
}

int Ranking::getWordFiles(const std::string_view& word, std::vector<int>& files) {
    auto nodes = Index::GetFilesIndex(data_index);

    if (!nodes->words()->LookupByKey(word)) {
        std::cerr << "Documnets don't contain \"" << word << "\"\n";
        return 1;
    }
    size_t len = nodes->words()->LookupByKey(word)->files()->size();

    for (int i = 0; i < len; ++i) {
        files.push_back((nodes->words()->LookupByKey(word)->files()->begin() + i)->id());
    }
    return 0;
}

void Ranking::pushInStack(std::stack<Node>& s, const std::string_view& word_view) {
    Node cur_node = s.top();
    s.pop();

    std::vector<int> word_files;
    getWordFiles(word_view, word_files);

    std::vector<int> result;
    getOperationResult(cur_node.files, word_files, result, cur_node.oper);
    
    cur_node.files = result;
    s.push(cur_node);
}

void Ranking::getOperations(const std::string& path, std::vector<std::string>& words, std::vector<Operation>& oper) {
    std::ifstream input_file(path);

    std::string word;

    while (input_file >> word) {

        while (word[0] == '(') {
            oper.push_back(Operation::LPAREN);
            word = word.substr(1);
        }
        size_t i = 1;
        size_t len = word.size();
        while ((len - i) >= 0 && word[len - i] == ')') {
            ++i;
        }
        --i;

        if (len - i > 0 && word == "AND") {
            oper.push_back(Operation::AND);
            
        } else if (len - i > 0 && word == "OR") {
            oper.push_back(Operation::OR);
        } else {
            oper.push_back(Operation::WORD);
            words.push_back(word.substr(0, len - i));
        }

        while (i > 0) {
            oper.push_back(Operation::RPAREN);
            --i;
        }
    }

    input_file.close();
}

void Ranking::getRequestFiles(const std::string& file, std::vector<int>& files, std::vector<std::string>& words) {
    std::stack<Node> s;
    s.push(Node{});
    
    std::vector<Operation> oper;

    getOperations(file, words, oper);

    std::ifstream input_file(file);

    for (int i = 0, j = 0; i < oper.size(); ++i) {
        if (oper[i] == Operation::LPAREN) {
            Node new_node;
            s.push(new_node);
        } else if (oper[i] == Operation::RPAREN && s.size() > 1) {
            Node cur_node = s.top(); s.pop();
            Node prev_node = s.top(); s.pop();

            std::vector<int> result;

            getOperationResult(cur_node.files, prev_node.files, result, prev_node.oper);
            cur_node.files.clear();
            cur_node.files = result;

            s.push(cur_node);
        } else if (oper[i] == Operation::RPAREN && s.size() <= 1) {

        } else if (oper[i] == Operation::AND) {
            Node cur_node = s.top();
            s.pop();

            cur_node.oper = Operation::AND;
            s.push(cur_node);
        } else if (oper[i] == Operation::OR) {
            Node cur_node = s.top();
            s.pop();

            cur_node.oper = Operation::OR;
            s.push(cur_node);
        } else {
            pushInStack(s, words[j]);
            ++j;
        }
    }
    files.clear();

    files = s.top().files;
    s.pop();

    input_file.close();
}

double Ranking::getTFIWord(const std::string& word, int file_id, int d_avg, int N, int dl) {
    auto nodes = Index::GetFilesIndex(data_index);
    auto files = nodes->words()->LookupByKey(word)->files();
    auto file = files->LookupByKey(file_id);

    double TF_IDF = 0;

    if (!file) {
        return TF_IDF;
    }

    int df = file->rows()->size();
    int tf = files->size();

    TF_IDF = ((tf * (K + 1)) / (tf + K * (1 - B + B * (dl / d_avg)))) * std::log((N - df + 0.5) / (df + 0.5));

    return TF_IDF;
}

int Ranking::getTFI(const std::vector<std::string>& words, int file, int d_avg, int N, int dl) {
    double TF_IDF = 0;
    for (const auto& word : words) {
        TF_IDF += getTFIWord(word, file, d_avg, N, dl);
    }

    return TF_IDF;
}

std::vector<int> Ranking::getBestFiles(const std::vector<std::string>& words, const std::vector<int>& files_vec, size_t amount_files) {
    auto files = Files::Getid_files(data_files_name);

    int d_avg = files->d_avg();
    int N = files->files()->size();

    std::priority_queue<std::pair<double, int>> qp;

    for (const auto& file : files_vec) {
        int dl = files->files()->LookupByKey(file)->dl();

        double rank = getTFI(words, file, d_avg, N, dl);

        qp.push({rank, file});
    }

    std::vector<int> result;

    for (int i = 0; i < amount_files && qp.size(); ++i) {
        result.push_back(qp.top().second);
        qp.pop();
    }

    return result;
}

std::string Ranking::getFileName(int file) {
    auto files = Files::Getid_files(data_files_name);;

    return files->files()->LookupByKey(file)->name()->c_str();
}

void Ranking::getRows(const std::vector<std::string>& words, int file, std::set<int>& rows) {
    auto nodes = Index::GetFilesIndex(data_index);
    

    for (const auto& word : words) {
        auto word_fbs = nodes->words()->LookupByKey(word);

        if (!word_fbs) {
            continue;
        }

        auto file_fbs = word_fbs->files()->LookupByKey(file);

        if (!file_fbs) {
            continue;
        }
        
        auto rows_word = file_fbs->rows();

        for (size_t i = 0; i < rows_word->size(); ++i) {
            rows.insert(*(rows_word->begin() + i));
        }
    }   
}

void Ranking::ranking(size_t amount_files) {
    std::vector<int> files;
    std::vector<std::string> words;
    getRequestFiles("request", files, words);

    std::vector<int> best_files = getBestFiles(words, files, amount_files);

    printRows(words, best_files);
}

void Ranking::printRows(std::vector<std::string>& words, std::vector<int>& best_files) {
    for (size_t i = 0; i < best_files.size(); ++i) {
        std::set<int> rows;
        getRows(words, best_files[i], rows);
        std::cout << getFileName(best_files[i]) << "\t:\t";

        size_t cnt = 0;
        for (const auto& row : rows) {
            std::cout << row;
            ++cnt;
            if (cnt < rows.size()) std::cout << ", ";
        }
        std::cout << '\n';
    }
}
