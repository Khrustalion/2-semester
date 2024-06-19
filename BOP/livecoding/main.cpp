/*
    Использовать исключительно для отладки. Комитить изменения этого файла запрещено!
*/

#include "task_1.h"
#include "task_2.h"
#include <vector>
#include <set>
#include <list>

int main(int, char**){
    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    /*
    1 a
    2 b
    3 c
    4 d
    */
    for(auto value : pythonzip(l, v)) {
        std::cout << value.first << " " << value.second << std::endl;
    }
}
