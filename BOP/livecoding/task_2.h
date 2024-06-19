#include <iostream>
#include <any>


template <typename It1, typename It2>
class iteratorZip {
    public:
    iteratorZip() = default;

    iteratorZip(const It1& it1, const It2& it2, const It1& it1_e, const It2& it2_e) {
        cur_it1 = it1;
        cur_it2 = it2;
        end_it1 = it1_e;
        end_it2 = it2_e;
    }

    std::pair<typename It1::value_type, typename It2::value_type> operator*() {
        return std::pair(*cur_it1, *cur_it2);
    }

    bool operator==(const iteratorZip& other) {
        if (end) {
            return false;
        }
        return ((*cur_it1) == (*other.cur_it1)) && ((*cur_it2) == (*other.cur_it2));
    }
    bool operator!=(const iteratorZip& other) {
        if (end) {
            return true;
        }
        return (cur_it1 != (other.cur_it1)) || ((cur_it2) != (other.cur_it2));
    }

    iteratorZip<It1, It2> operator++() {
        if (cur_it1 != end_it1) ++cur_it1;
        else end = true;
        if (cur_it2 != end_it2) ++cur_it2;
        else end = true;

        return *this;
    }

    iteratorZip<It1, It2> operator++(int n) {
        if (cur_it1 != end_it1) cur_it1++;
        if (cur_it2 != end_it2) cur_it2++;

        return *this;
    }

    private:
    bool end = false;
    It1 cur_it1;
    It2 cur_it2;
    It1 end_it1;
    It2 end_it2;
};

template <typename T1, typename T2>
class pythonzip {
    public:

    pythonzip() = default;

    pythonzip(T1& cont1, T2& cont2) {
        begin_it1 = cont1.begin();
        end_it1 = cont1.end();
        begin_it2 = cont2.begin();
        end_it2 = cont2.end();
    }

    template <typename T1_, typename T2_>
    pythonzip(const pythonzip<T1_, T2_>& other) {
        begin_it1 = other.begin_it1;
        begin_it2 = other.begin_it2;
        end_it1 = other.end_it1;
        end_it2 = other.end_it2;
    }

    template <typename T1_, typename T2_>
    auto operator=(const pythonzip<T1_, T2_>& other) {
        begin_it1 = other.begin_it1;
        begin_it2 = other.begin_it2;
        end_it1 = other.end_it1;
        end_it2 = other.end_it2;
    }

    template <typename T1_, typename T2_>
    auto operator=(pythonzip<T1_, T2_>&& other) {
        begin_it1 = other.begin_it1;
        begin_it2 = other.begin_it2;
        end_it1 = other.end_it1;
        end_it2 = other.end_it2;
    }

    iteratorZip<typename T1::iterator, typename T2::iterator> begin() {
        return iteratorZip(begin_it1, begin_it2, end_it1, end_it2);
    }  

    iteratorZip<typename T1::iterator, typename T2::iterator> end() {
        return iteratorZip(end_it1, end_it2,  end_it1, end_it2);
    } 

    private:
    typename T1::iterator begin_it1;
    typename T2::iterator begin_it2;
    typename T1::iterator end_it1;
    typename T2::iterator end_it2;
};