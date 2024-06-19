/*

*/
#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
class iterColumn {
public:
    iterColumn(T* ptr_vec, int cur_col, int cols) : ptr_vec_{ptr_vec}, cur_col_{cur_col}, cols_{cols} {};

    iterColumn operator++() {
        ++cur_col_;

        return *this;
    }

    iterColumn begin() {
        return iterColumn(ptr_vec_, 0, cols_);
    }

    iterColumn end() {
        return iterColumn(ptr_vec_, cols_, cols_);
    }

    T operator*() {
        return *(ptr_vec_ + cur_col_);
    }

    bool operator==(const iterColumn& other) {
        return ptr_vec_ == other.ptr_vec_ && cur_col_ == other.cur_col_ && cols_ == other.cur_col_;
    }

    bool operator!=(const iterColumn& other) {
        return !((*this) == other);
    }

    T operator<<(const std::ofstream& stream) {
        stream << **this;
    }

private:
    T* ptr_vec_;
    int cur_col_;
    int cols_;
};

template <typename T>
class iterRow {
public:
    iterRow(T* ptr_vec, int cur_row, int rows, int cols) : ptr_vec_{ptr_vec}, cur_row_{cur_row}, rows_{rows}, cols_{cols} {};

    iterRow operator++() {
        ++cur_row_;

        return *this;
    }

    iterRow begin() const {
        return iterRow(ptr_vec_, 0, rows_, cols_);
    }

    iterRow end() const {

        return iterRow(ptr_vec_, rows_, rows_, cols_);
    }

    bool operator==(const iterRow& other) const {
        return ptr_vec_ == other.ptr_vec_ && cur_row_ == other.cur_row_ && rows_ == other.rows_ && cols_ == other.cols_; 
    }

    bool operator!=(const iterRow& other) {
        return !((*this) == other);
    }

    iterColumn<T> cols() {
        return iterColumn<T>(ptr_vec_ + cur_row_ * cols_, 0, cols_);
    }

    iterRow<T> operator*() {
        return *this;
    }

private:
    T* ptr_vec_;
    int cur_row_;
    int rows_;
    int cols_;
};

template<typename T>
class row {
public:
    row(T* ptr_vec, int row, int cols) : ptr_vec_{ptr_vec}, row_{row}, cols_{cols} {};

    T operator[](int i) {
        return *(ptr_vec_ + cols_ * row_ + i);
    }

private:
    T* ptr_vec_;
    int row_;
    int cols_;
};

template <typename T>
class multiarray {
public:
    multiarray(T* ptr_vec, int rows, int cols) : ptr_vec_{ptr_vec}, rows_{rows}, cols_{cols} {}

    row<T> operator[](int i) {
        return row<T>{ptr_vec_, i, cols_};
    }

    iterRow<T> rows() {
        return iterRow<T>(ptr_vec_, 0, rows_, cols_);
    }

    int numberRows() {
        return rows_;
    }

    int numberCols() {
        return cols_;
    }

private:
    T* ptr_vec_;
    int rows_;
    int cols_;
};

struct MyClass {
    MyClass(int x, int y, const std::string& z ) : x(x), y(y), z(z) {}
    int x;
    int y;
    std::string z;
};

struct Creater {
    template <typename T1, typename ... Args>
    static T1 create(Args ... args) {
        return T1(std::forward<Args>(args)...);
    }
};

int main() {
    MyClass x = Creater::create<MyClass>(1, 2, "HELLO");
    int y = Creater::create<int>(1);
    std::vector<int> vctr = Creater::create<std::vector<int>>({1, 2, 3, 4});
    /*
    std deprecated
    */
    std::vector v = {0,1,2,3,4,5};

    multiarray a{v.data(), 3, 2};

    assert(a[2][0] == 4);

    assert(a[1][1] == 3);

    for(int i = 0; i < a.numberRows(); ++i)
        for(int j = 0; j < a.numberCols(); ++j)
            std::cout << a[i][j]; // 012345

    // std::cout << '\n';

    // std::vector v1 = {"1 ", "2 ", "3 ", "4 ", "5 ", "6 "};

    // multiarray b(v1.data(), 3, 2);

    // for(int i = 0; i < b.numberRows(); ++i)
    //     for(int j = 0; j < b.numberCols(); ++j)
    //         std::cout << b[i][j]; 

    // std::cout << '\n';

    // for (auto row : a.rows())
    //     for (auto col : row.cols()) 
    //         std::cout << col;
    return 0;

}