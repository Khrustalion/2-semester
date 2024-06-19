#include <iostream>

template <typename T, typename Func>
bool half_of(T range, Func f) {
    size_t cnt = 0;

    for (auto val : range) {
        if (f(val)) {
            ++cnt;
        }
    }

    return cnt >= (std::distance(range.begin(), range.end()) + 1) / 2;
}
