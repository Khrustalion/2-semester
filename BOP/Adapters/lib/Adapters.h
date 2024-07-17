#pragma once
#include <iostream>
#include <vector>

template<typename F_t>
class transform {
    private:
    F_t transform_func_;
    bool active_ = false;

    public:
    transform(F_t transform_func) : transform_func_{transform_func} {
        active_ = true;
    };

    transform() {};

    F_t getTransformFunc() {
        return transform_func_;
    }

    bool isActive() {
        return active_;
    }
};

template<typename It, typename OutType>
class StandartTransformFunc {
    public:
    OutType operator()(const It& it) {
        return *it;
    }

    OutType operator()(const OutType t) {
        return t;
    }
};

template<typename It>
class StandartFilterformFunc {
    public:
    bool operator()(const It& t) {
        return true;
    }
};

template<typename It, typename OutType>
class KeysFunc {
    public:
    KeysFunc() {};

    OutType operator()(const It& it) {
        return it->first;
    }
};  

template<typename It, typename OutType>
class ValuesFunc {
    public:
    ValuesFunc() {};

    OutType operator()(const It& it) {
        return it->second;
    }
};

template<typename F_f>
class filter {
    private:
    F_f filter_func_;

    bool active_ = false;

    public:
    filter(const F_f& filter_func) : filter_func_{filter_func} {
        active_ = true;
    };

    filter() {};

    F_f getFilterFunc() {
        return filter_func_;
    }

    bool isActive() {
        return active_;
    }
};

class take {
    private:
    int quantity_take_ = -1;

    public:
    take() {};

    take(int quantity_take) : quantity_take_{quantity_take} {};

    int getQuantityTake() {
        return quantity_take_;
    }

    bool isActive() {
        return quantity_take_ != -1;
    }
};

class drop {
    private:
    int quantity_drop_ = -1;

    public:
    drop() {};

    drop(int quantity_drop) : quantity_drop_{quantity_drop} {};

    int getQuantityDrop() {
        return quantity_drop_;
    }

    bool isActive() {
        return quantity_drop_ != -1;
    }
};

class keys {};

class values {};

class reverse {};

template<typename It, typename T, typename value_type, typename F_t = StandartTransformFunc<It, value_type>, typename F_f = StandartFilterformFunc<value_type>, typename F_ = StandartTransformFunc<It, value_type>>
class Adapters {
    private:
    transform<F_t> transform_;
    filter<F_f> filter_;
    take take_;
    drop drop_;
    bool reverse_ = false;
    F_ get_value_;
    int current_number_it_ = 0;
    It begin_;
    It current_it_;
    It end_;

    public:
    Adapters(It begin, It end, It current_it, transform<F_t> new_transform, filter<F_f> new_filter, take new_take, drop new_drop, F_ new_get_value, bool new_reverse) 
      : begin_{begin},
        end_{end},
        current_it_{current_it},
        transform_{new_transform},
        filter_{new_filter},
        take_{new_take},
        drop_{new_drop},
        get_value_{new_get_value},
        reverse_{new_reverse}   {
        if (drop_.isActive()) {
            for (int i = drop_.getQuantityDrop(); i > 0; --i) {
                if (current_it_ != end_) {
                    if (reverse_) {
                        --current_it;
                    } else {
                        ++current_it_;
                    }
                }
            }
        }
        if (filter_.isActive()) {
            while (current_it_ != end_ && (!filter_.getFilterFunc()(get_value_(current_it_)))) {
                if (reverse_) {
                    --current_it;
                } else {
                    ++current_it_;
                }
            }
        }
    };

    Adapters(transform<F_t> new_transform, It begin, It end) : transform_{new_transform}, begin_{begin}, end_{end} {
        current_it_ = begin_;
    };

    Adapters(filter<F_f> new_filter, It begin, It end) : filter_{new_filter}, begin_{begin}, end_{end} {
        current_it_ = begin_;
        while (current_it_ != end_ && (!filter_.getFilterFunc()(get_value_(current_it_)))) {
            ++current_it_;
        }
    };

    Adapters(take new_take, It begin, It end) : take_{new_take}, begin_{begin}, end_{end} {
        current_it_ = begin_;
    };

    Adapters(drop new_drop, It begin, It end) : drop_{new_drop}, begin_{begin}, end_{end} {
        current_it_ = begin_;
        for (int i = drop_.getQuantityDrop(); i > 0; --i) {
            if (current_it_ != end_) {
                ++current_it_;
            }
        }
    };

    Adapters(keys new_keys, It begin, It end) : begin_{begin}, end_{end} {
        current_it_ = begin_;
    }

    Adapters(values new_values, It begin, It end) : begin_{begin}, end_{end} {
        current_it_ = begin_;
    }

    Adapters(reverse new_reverse, It begin, It end) : begin_{begin}, end_{end} {
        current_it_ = end_;
        --current_it_;
        reverse_ = true;
    }

    template<typename F_f_>
    Adapters<It, T, value_type, F_t, F_f_, F_> operator|(filter<F_f_> new_filter) {
        Adapters<It, T, value_type, F_t, F_f_, F_> new_adapters(begin_, end_, current_it_, transform_, new_filter, take_, drop_, get_value_, reverse_);

        return new_adapters;
    }

    template<typename F_t_>
    Adapters<It, T, value_type, F_t_, F_f, F_> operator|(transform<F_t_> new_transform) {
        Adapters<It, T, value_type, F_t_, F_f, F_> new_adapters(begin_, end_, current_it_, new_transform, filter_, take_, drop_, get_value_, reverse_);

        return new_adapters;
    }

    Adapters<It, T, value_type, F_t, F_f, F_> operator|(take new_take) {
        Adapters<It, T, value_type, F_t, F_f, F_> new_adapters(begin_, end_, current_it_, transform_, filter_, new_take, drop_, get_value_, reverse_);

        return new_adapters;
    }

    Adapters<It, T, value_type, F_t, F_f, F_> operator|(drop new_drop) {
        Adapters<It, T, value_type, F_t, F_f, F_> new_adapters(begin_, end_, current_it_, transform_, filter_, take_, new_drop, get_value_, reverse_);

        return new_adapters;
    }

    Adapters<It, T, value_type, F_t, F_f, F_> operator|(reverse new_reverse) {
        Adapters<It, T, value_type, F_t, F_f, F_> new_adapters(begin_, end_, current_it_, transform_, filter_, take_, drop_, get_value_, true);

        return new_adapters;
    }

    Adapters begin() {
        if (reverse_) {
            It new_begin = end_;
            --new_begin;
            return Adapters(begin_, end_, new_begin, transform_, filter_, take_, drop_, get_value_, reverse_);
        }
        return Adapters(begin_, end_, begin_, transform_, filter_, take_, drop_, get_value_, reverse_);
    }

    Adapters end() {
        return Adapters(begin_, end_, end_, transform_, filter_, take_, drop_, get_value_, reverse_);
    }

    bool operator==(const Adapters& rhs) {
        return rhs.current_it_ == current_it_;
    }

    bool operator!=(const Adapters& rhs) {
        return !(rhs.current_it_ == current_it_);
    }

    Adapters operator++() {
        if (reverse_ && current_it_ == begin_) {
            current_it_ = end_;
            return *this;
        }
        ++current_number_it_;
        if (current_number_it_ == take_.getQuantityTake())  {
            current_it_ = end_;
            return *this;
        }
        if (reverse_) {
            --current_it_;
        } else {
            ++current_it_;
        }
        if (filter_.isActive()) {
            while ((current_it_ != end_) && (!filter_.getFilterFunc()(get_value_(current_it_)))) {
            ++current_it_;
            }
        }
        return *this;
    }

    value_type operator*() { 
        if (transform_.isActive()) {
            return transform_.getTransformFunc()(get_value_(current_it_));
        }
        return get_value_(current_it_);
    }
};

template<typename T, typename F_t>
    requires requires(T) {typename T::const_iterator; typename T::value_type; }
Adapters<typename T::const_iterator, T, typename T::value_type, F_t, StandartFilterformFunc<typename T::value_type>, StandartTransformFunc<typename T::const_iterator, typename T::value_type>> operator|(const T& container, transform<F_t> transform_) {
    return Adapters<typename T::const_iterator, T, typename T::value_type, F_t, StandartFilterformFunc<typename T::value_type>, StandartTransformFunc<typename T::const_iterator, typename T::value_type>>(transform_, container.begin(), container.end());
}

template<typename T, typename F_f>
    requires requires(T) {typename T::const_iterator; typename T::value_type; }
Adapters<typename T::const_iterator, T, typename T::value_type, StandartTransformFunc<typename T::const_iterator, typename T::value_type>, F_f, StandartTransformFunc<typename T::const_iterator, typename T::value_type>> operator|(const T& container, filter<F_f> filter_) {
    return Adapters<typename T::const_iterator, T, typename T::value_type, StandartTransformFunc<typename T::const_iterator, typename T::value_type>, F_f, StandartTransformFunc<typename T::const_iterator, typename T::value_type>>(filter_, container.begin(), container.end());
}

template<typename T, typename Adapter>
    requires requires(T) {typename T::const_iterator; typename T::value_type; }
Adapters<typename T::const_iterator, T, typename T::value_type> operator|(const T& container, Adapter adapter_) {
    return Adapters<typename T::const_iterator, T, typename T::value_type>(adapter_, container.begin(), container.end());
}

template<typename T>
    requires requires(T) {typename T::key_type; typename T::mapped_type; typename T::const_iterator; }
Adapters<typename T::const_iterator, T, typename T::key_type, StandartTransformFunc<typename T::const_iterator, typename T::key_type>, StandartFilterformFunc<typename T::key_type>, KeysFunc<typename T::const_iterator, typename T::key_type>> operator|(const T& container, keys keys_) {
    return Adapters<typename T::const_iterator, T,  typename T::key_type, StandartTransformFunc<typename T::const_iterator, typename T::key_type>, StandartFilterformFunc<typename T::key_type>, KeysFunc<typename T::const_iterator, typename T::key_type>>(keys_, container.begin(), container.end());
}

template<typename T>
    requires requires(T) {typename T::key_type; typename T::mapped_type; typename T::const_iterator; }
Adapters<typename T::const_iterator, T, typename T::mapped_type, StandartTransformFunc<typename T::const_iterator, typename T::mapped_type>, StandartFilterformFunc<typename T::mapped_type>, ValuesFunc<typename T::const_iterator, typename T::mapped_type>> operator|(const T& container, values values_) {
    return Adapters<typename T::const_iterator, T,  typename T::mapped_type, StandartTransformFunc<typename T::const_iterator, typename T::mapped_type>, StandartFilterformFunc<typename T::mapped_type>, ValuesFunc<typename T::const_iterator, typename T::mapped_type>>(values_, container.begin(), container.end());
}

template<typename T>
    requires requires(T) {typename T::const_iterator; typename T::value_type; }
Adapters<typename T::const_iterator, T, typename T::value_type, StandartTransformFunc<typename T::const_iterator, typename T::value_type>, StandartFilterformFunc<typename T::value_type>, ValuesFunc<typename T::const_iterator, typename T::value_type>> operator|(const T& container, reverse reverse_) {
    return Adapters<typename T::const_iterator, T,  typename T::value_type, StandartTransformFunc<typename T::const_iterator, typename T::value_type>, StandartFilterformFunc<typename T::value_type>, ValuesFunc<typename T::const_iterator, typename T::value_type>>(reverse_, container.begin(), container.end());
}
