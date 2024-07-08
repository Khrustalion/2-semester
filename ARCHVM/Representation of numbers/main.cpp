#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <ostream>
#include <ios>
#include <cstdlib>

struct ArgumentsFixed {
    int A;
    int B;
    int rounding;
    long long  number_1;
    char operation = 0;
    long long  number_2;
};

struct ArgumentsFloat {
    char type;
    int rounding;
    long long  number_1;
    char operation = 0;
    long long  number_2;
};

bool isNumber(char* str) {
    size_t size = std::strlen(str);
    for (int i = 0; i < size; ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

template<int size_man, int size_exp, int OFFSET, int MIN_EXP, int MAX_EXP, int NAN_, int PINF>
class FloatPoint {
    public:
        FloatPoint(long long number, int rounding) {
            if (number & (static_cast<int64_t>(1) << (size_man + size_exp))) {
                neg_sign_ = true;
            }
            rounding_ = rounding;
            exponent_ = ((static_cast<int64_t>(1) << size_exp) + (number >> size_man) % (static_cast<int64_t>(1) << size_exp)) % (static_cast<int64_t>(1) << size_exp) - OFFSET;
            if (exponent_ == MIN_EXP) {
                is_null_ = true;
                ++exponent_;
            }
            mantissa_ = (number % (static_cast<int64_t>(1) << size_man));
        }

        FloatPoint operator-() {
            long long new_exponent = is_null_ ? MIN_EXP : exponent_;
            long long number = ((new_exponent + OFFSET) << size_man) | (mantissa_);
            if (!neg_sign_) {
                number |= (static_cast<int64_t>(1) <<  (size_man + size_exp));
            }
            return FloatPoint(number, rounding_);
        }

        FloatPoint operator*(FloatPoint rhs) {
            if (rhs.isNan() || this->isNan()) {
                return FloatPoint(NAN_, 0);
            }

            if (neg_sign_ && (!rhs.neg_sign_)) {
                return -((-(*this)) * rhs);
            }
            else if ((!neg_sign_) && rhs.neg_sign_) {
                return -((*this) * (-rhs));
            }
            else if (neg_sign_ && rhs.neg_sign_) {
                return (-(*this)) * (-rhs);
            }
            else if (this->isZero() && rhs.isZero()) {
                return (*this);
            }
            else if ((this->isInf() && rhs.isZero()) || (this->isZero() && rhs.isInf())) {
                return FloatPoint(NAN_, rounding_);
            }
            else if (rhs.isZero() || this->isZero()) {
                return FloatPoint(0, rounding_);
            }

            long long lhs_number = mantissa_;
            long long rhs_number = rhs.mantissa_;
            long long new_exponent = exponent_ + rhs.exponent_;

            if (!is_null_) {
                lhs_number |= (static_cast<int64_t>(1) << size_man);
            }
            if (!rhs.is_null_) {
                rhs_number |= (static_cast<int64_t>(1) << size_man);
            }
            long long result = rounding(size_man, rhs_number * lhs_number);

            if (result >= (static_cast<int64_t>(1) << (size_man + 1))) {
                ++new_exponent;
                result = rounding(1, result);
                if (new_exponent >= MAX_EXP) {
                    return FloatPoint(((static_cast<int64_t>(1) << (size_man + size_exp)) - 1) ^ (1 << size_man), rounding_);
                }
            } 
            else if (result < (static_cast<int64_t>(1) << size_man)) {
                int cnt = size_man;
                while ((!(result & (static_cast<int64_t>(1) << size_man))) && (new_exponent != MIN_EXP + 1)) {
                    --cnt;
                    result = rounding(cnt, rhs_number * lhs_number);
                    --new_exponent;
                }
                if ((new_exponent == (MIN_EXP + 1)) && (!(result & (static_cast<int64_t>(1) << size_man)))) {
                    new_exponent = MIN_EXP;
                }
            }

            if (new_exponent >= MAX_EXP) {
                return FloatPoint(((static_cast<int64_t>(1) << (size_man + size_exp)) - 1) ^ (1 << size_man), rounding_);
            }

            if (new_exponent == MIN_EXP && result >= (static_cast<int64_t>(1) << (size_man))) {
                result >>= 1;
            }

            if (new_exponent < MIN_EXP) {
                while ((result != 0) && (new_exponent != MIN_EXP)) {
                    result >>= 1;
                    ++new_exponent;
                }
                result >>= 1;

                if (result & (1 << size_man)) {
                    ++new_exponent;
                }

                if (result == 0) {
                    return FloatPoint(0, rounding_);
                }
            }


            result %= (static_cast<int64_t>(1) << size_man);

            long long number = ((new_exponent + OFFSET) << size_man) | result;

            return FloatPoint(number, rounding_);
        }

        FloatPoint operator/(FloatPoint rhs) {
            if (rhs.isNan() || this->isNan() || (this->isZero() && rhs.isZero()) || (this->isInf() && rhs.isInf())) {
                return FloatPoint(NAN_, 0);
            }

            if (neg_sign_ && (!rhs.neg_sign_)) {
                return -((-(*this)) / rhs);
            }
            else if ((!neg_sign_) && rhs.neg_sign_) {
                return -((*this) / (-rhs));
            }
            else if (neg_sign_ && rhs.neg_sign_) {
                return (-(*this)) / (-rhs);
            }

            if (rhs.isZero()) {
                return FloatPoint(PINF, rounding_);
            }
            else if (this->isZero()) {
                return (*this);
            }

            long long lhs_number = mantissa_;
            long long rhs_number = rhs.mantissa_;
            int new_exponent = exponent_ - rhs.exponent_;

            if (!is_null_) {
                lhs_number |= (static_cast<int64_t>(1) << size_man);
            }
            if (!rhs.is_null_) {
                rhs_number |= (static_cast<int64_t>(1) << size_man);
            }

            long long result = (lhs_number << size_man) / rhs_number;
            
            if (result < (static_cast<int64_t>(1) << size_man)) {
                long long ind = (size_man + 1);
                while (result < (static_cast<int64_t>(1) << size_man) && (new_exponent != (MIN_EXP + 1))) {
                    result = (lhs_number << ind) / rhs_number;
                    --new_exponent;
                    ++ind;
                }

                if ((new_exponent == (MIN_EXP + 1)) && (result < (static_cast<int64_t>(1) << size_man))) {
                    new_exponent = MIN_EXP;
                }
            }
            else if (result >= (static_cast<int64_t>(1) << (size_man + 1))) {
                int max_ind = (size_man + 1);
                while (result >= (static_cast<int64_t>(1) << max_ind)) {
                    ++max_ind;
                }
                result = rounding(max_ind - 1 - size_man, result);
                new_exponent += max_ind - 1 - size_man;
            }

            if (new_exponent >= MAX_EXP) {
                return FloatPoint(((static_cast<int64_t>(1) << (size_man + size_exp)) - 1) ^ (1 << size_man), rounding_);
            }

            if (new_exponent == MIN_EXP && result >= (static_cast<int64_t>(1) << (size_man))) {
                result >>= 1;
            }

            if (new_exponent < MIN_EXP) {
                while ((result != 0) && (new_exponent != MIN_EXP)) {
                    result >>= 1;
                    ++new_exponent;
                }
                result >>= 1;

                if (result >= (1 << size_man)) {
                    ++new_exponent;
                }

                if (result == 0) {
                    return FloatPoint(0, rounding_);
                }
            }

            result %= (static_cast<int64_t>(1) << size_man);

            long long number = ((new_exponent + OFFSET) << size_man) | result;

            return FloatPoint(number, rounding_);

        }

        FloatPoint operator+(FloatPoint rhs) {
            if (rhs.isNan() || this->isNan()) {
                return FloatPoint(NAN_, 0);
            }

            if (neg_sign_ && rhs.neg_sign_) {
                return -((-(*this)) + (-rhs));
            }
            else if (neg_sign_) {
                return rhs - (-(*this));
            }
            else if (rhs.neg_sign_) {
                return (*this) - (-rhs);
            }

            else if (rhs.isInf() && this->isInf() && (rhs.isNeg() ^ neg_sign_)) {
                return FloatPoint(NAN_, 0);
            }
            else if (rhs.isInf() && this->isInf()) {
                return rhs;
            }

            int new_exponent = std::max(exponent_, rhs.exponent_);
            long long new_mantissa = 0;
            long long lhs_mantissa = mantissa_;
            long long rhs_mantissa = rhs.mantissa_;

            bool answer_is_null = false;


            if (rhs.exponent_ < new_exponent){
                rhs_mantissa = rounding(new_exponent - rhs.exponent_, rhs.mantissa_);
                if ((!rhs.isNullExp()) && ((new_exponent - rhs.exponent_) <= size_man)) {
                    rhs_mantissa |= (static_cast<int64_t>(1) << (size_man - (new_exponent - rhs.exponent_)));
                }
                if (!is_null_) {
                    new_mantissa += (static_cast<int64_t>(1) << size_man);
                }
            }

            else if (exponent_ < new_exponent) {
                lhs_mantissa = rounding(new_exponent - exponent_, mantissa_);
                if ((!is_null_) && ((new_exponent - exponent_) <= size_man)) {
                    lhs_mantissa |= (static_cast<int64_t>(1) << (size_man - new_exponent + exponent_));
                }
                if (!rhs.isNullExp()) {
                    new_mantissa += (static_cast<int64_t>(1) << size_man);
                }
            }
            else {
                if (!rhs.isNullExp()) {
                    new_mantissa += (static_cast<int64_t>(1) << size_man);
                }
                if (!this->isNullExp()) {
                    new_mantissa += (static_cast<int64_t>(1) << size_man);
                }
                else if (rhs.is_null_ && is_null_) {
                    answer_is_null = true;
                }
            }

            new_mantissa += lhs_mantissa + rhs_mantissa;

            if (new_mantissa >= (static_cast<int64_t>(1) << (size_man + 1))) {
                ++new_exponent;
                new_mantissa = rounding(1, new_mantissa);
            } 
            else if (answer_is_null && (new_mantissa < (static_cast<int64_t>(1) << size_man))) {
                --new_exponent;
            }

            if (new_exponent >= MAX_EXP) {
                return FloatPoint(((static_cast<int64_t>(1) << (size_man + size_exp)) - 1) ^ (1 << size_man), rounding_);
            }

            new_mantissa %= (static_cast<int64_t>(1) << size_man);
            long long number = ((new_exponent + OFFSET) << size_man) | (new_mantissa);
            return FloatPoint(number, rounding_);
        }

        FloatPoint operator-(FloatPoint rhs) {
            if (rhs.isNan() || this->isNan()) {
                return FloatPoint(NAN_, 0);
            }
            else if (rhs.isInf() && this->isInf() && (rhs.isNeg() ^ neg_sign_)) {
                return rhs;
            }
            else if (rhs.isInf() && this->isInf()) {
                return FloatPoint(NAN_, 0);
            }

            if (neg_sign_ && rhs.neg_sign_) {
                return (-rhs) + (*this);
            }
            else if (neg_sign_) {
                return -((-(*this)) + rhs);
            }
            else if (rhs.neg_sign_) {
                return (*this) + (-rhs);
            }
            else if ((exponent_ < rhs.exponent_) || (is_null_ && (!rhs.is_null_))) {
                return -(rhs - (*this));
            }
            else if ((exponent_ == rhs.exponent_) && (mantissa_ < rhs.mantissa_) && (!(is_null_ ^ rhs.is_null_))) {
                return -(rhs - (*this));
            }

            if (rhs.isZero() && this->isZero()) {
                return rhs;
            }
            else if (rhs.isZero()) {
                return (*this);
            }
            else if (this->isZero()) {
                return rhs;
            }
            
            int new_exponent = std::min(exponent_, rhs.exponent_);
            long long new_mantissa = 0;
            long long lhs_number = mantissa_;
            long long rhs_number = rhs.mantissa_;
            int new_offset = 0;

            bool answer_is_null = false;

            if (exponent_ == rhs.exponent_) {
                if (!rhs.isNullExp()) {
                    rhs_number |= (static_cast<int64_t>(1) << size_man);
                }
                if (!is_null_) {
                    lhs_number |= (static_cast<int64_t>(1) << size_man);
                }
                if (rhs.is_null_ && is_null_) {
                    answer_is_null = true;
                }
            }
            else if (exponent_ - rhs.exponent_ > size_man) {
                new_exponent = exponent_;
                if (!rhs.isZero()) {
                    if (lhs_number != 0) {
                        new_mantissa = (lhs_number - 1);
                    }
                    else {
                        new_mantissa = (1 << size_man) - 1;
                        --new_exponent;
                    }
                }
                
                long long number = ((new_exponent + OFFSET) << size_man) | new_mantissa;
                return (FloatPoint(number, rounding_));
            }
            else {
                new_offset = exponent_ - rhs.exponent_;

                if (!rhs.isNullExp()) {
                    rhs_number |=  (static_cast<int64_t>(1) << size_man);
                }
                if (!is_null_) {
                    lhs_number |= (static_cast<int64_t>(1) << size_man);
                }
                lhs_number <<= (new_offset);
            }
            
            long long result = (lhs_number - rhs_number);

            if (result == 0) {
                return FloatPoint(0, rounding_);
            }
            if (new_offset != 0) {
                int tmp = 0;
                while (result >= (static_cast<int64_t>(1) << tmp)) {
                    ++tmp;
                }

                new_exponent += tmp - size_man - 1;
            
                result = rounding(tmp - size_man - 1, result);
            }

            if (result < (static_cast<int64_t>(1) << size_man)) {
                while ((!(result & (static_cast<int64_t>(1) << size_man))) && (new_exponent != MIN_EXP + 1)) {
                    result <<= 1;
                    --new_exponent;
                }
                if ((new_exponent == (MIN_EXP + 1)) && (!(result & (static_cast<int64_t>(1) << size_man)))) {
                    new_exponent = MIN_EXP;
                }
            }

            if (new_exponent == MIN_EXP && result >= (static_cast<int64_t>(1) << (size_man))) {
                result >>= 1;
            }

            if (new_exponent < MIN_EXP) {
                while ((result != 0) && (new_exponent != MIN_EXP)) {
                    result >>= 1;
                    ++new_exponent;
                }
                result >>= 1;

                if (result >= (1 << size_man)) {
                    ++new_exponent;
                }

                if (result == 0) {
                    return FloatPoint(0, rounding_);
                }
            }

            result %= (static_cast<int64_t>(1) << size_man);

            long long number = ((new_exponent + OFFSET) << size_man) | result;

            return FloatPoint(number, rounding_);
        }

        bool isZero() {
            return is_null_ && (mantissa_ == 0);
        }

        bool isInf() {
            if ((exponent_ == MAX_EXP) && (mantissa_ == 0)) {
                return true;
            } 
            return false;
        }

        bool isNan() {
            if ((exponent_ == MAX_EXP) && (mantissa_ > 0)) {
                return true;
            }
            return false;
        }

        bool isNeg() {
            return neg_sign_;
        }

        bool isNullExp() {
            return is_null_;
        }

        long long getMantissa() {
            return mantissa_;
        }

        long long getExponent() {
            return exponent_;
        }

        long long rounding(int bit, long long mantissa) {
            if (rounding_ == 0) {
                return toward_zero(bit, mantissa);
            }
            return 0;
        }

    private:
        long long toward_zero(int bit, long long mantissa) {
            long long new_mantissa = 0;
            if (bit >= 0) {
                new_mantissa = (mantissa >> std::min(bit, size_man));
            }
            else {
                new_mantissa = (mantissa << std::min(-bit, size_man));
            }
            return new_mantissa;
        }
        
        int exponent_;
        long long mantissa_;
        int rounding_;
        bool is_null_ = false;
        bool neg_sign_ = false;
};

template<int size_man, int size_exp, int OFFSET, int MIN_EXP, int MAX_EXP, int NAN_, int PINF>
std::ostream& operator<<(std::ostream& stream, FloatPoint<size_man, size_exp, OFFSET, MIN_EXP, MAX_EXP, NAN_, PINF> num) {
    if (num.isNan()) {
        std::cout << "nan";
        return stream;
    }
    else if (num.isInf()) {
        if (num.isNeg()) {
            std::cout << "-";
        }
        std::cout << "inf";
        return stream;
    } 

    if (num.isNeg()) {
        std::cout << "-";
    }
    
    std::cout << "0x";
    if (num.isZero() && size_man == 10) {
        std::cout << "0.000p+0";
        return stream;
    }
    else if (num.isZero() && size_man == 23) {
        std::cout << "0.000000p+0";
        return stream;
    }
    std::cout << "1.";
    long long new_exponent = num.getExponent();
    long long new_mantissa = num.getMantissa();

    if (num.isNullExp()) {
        while (!(new_mantissa & (static_cast<int64_t>(1) << size_man))) {
            --new_exponent;
            new_mantissa <<= 1;
        }
    }

    int amount_digit = size_man == 10 ? 2 : 5;
    int offset = amount_digit == 2 ? 2 : 1;
    for (int i = amount_digit; i >= 0; --i) {
        long long tmp_mantissa = new_mantissa << offset;
        std::cout << std::hex << (tmp_mantissa >> (4 * i)) % (static_cast<int64_t>(1) << 4);
    }
    std::cout << "p";
    if (new_exponent >= 0) {
        std::cout << "+";
    }
    std::cout << std::dec << new_exponent;

    return stream;
}


class FixedPoint {
    public:
        FixedPoint(char A, char B, char rounding, long long number) : A_(A), B_(B), rounding_(rounding), number_(number) {
            number_ = number % (static_cast<int64_t>(1) << (A + B));

            if (number_ >= (static_cast<int64_t>(1) << (A_ + B_ - 1))) {
                number_ -= (static_cast<int64_t>(1) << (A_ + B_));
            }    


            if (B_ != 0) {
                int_part_ = ((static_cast<int64_t>(1) << A_) + (number_ >> B_) % (static_cast<int64_t>(1) << A_)) % (static_cast<int64_t>(1) << A_);
            }
            else {
                int_part_ = ((static_cast<int64_t>(1) << A_)  + number_ % (static_cast<int64_t>(1) << A_)) % (static_cast<int64_t>(1) << A_);
            }
            if (int_part_ >= (static_cast<int64_t>(1) << (A_ - 1))) {
                int_part_ -= (static_cast<int64_t>(1) << A_);
            }
            fractional_part_ = ((static_cast<int64_t>(1) << B_) + (number_ % (static_cast<int64_t>(1) << B_))) % (static_cast<int64_t>(1) << B_);
            if (int_part_ < 0) {
                neg_sign_ = true;
            }
            
        };

        FixedPoint operator+(FixedPoint& rhs) {
            return FixedPoint(A_, B_, rounding_, number_ + rhs.number_);
        }

        FixedPoint operator-(FixedPoint& rhs) {
            if (number_ < rhs.number_) {
                return FixedPoint(A_, B_, rounding_, -(rhs.number_ - number_));
            }
            return FixedPoint(A_, B_, rounding_, number_ - rhs.number_);
        }

        FixedPoint operator*(FixedPoint& rhs) {
            long long answer = ((static_cast<int64_t>(1) << (A_ + B_)) + (((number_ * rhs.number_) >> B_) % (static_cast<int64_t>(1) << (A_ + B_))));
            long long least_significant_bits = (number_ * rhs.number_) % (static_cast<int64_t>(1) << B_);

            if ((least_significant_bits != 0) && (rounding_ == 0) && (neg_sign_ ^ rhs.neg_sign_)) {
                ++answer;
            }
            else if ((least_significant_bits != 0) && (rounding_ == 1)) {
                if ((least_significant_bits == (static_cast<int64_t>(1) << (B_ - 1))) && (answer % 2)) {
                    ++answer;
                }
                else if (least_significant_bits > (static_cast<int64_t>(1) << (B_ - 1))) {
                    ++answer;
                }
            }
            else if ((least_significant_bits != 0) && (rounding_ == 2)) {
                ++answer;
            }
            else if ((least_significant_bits != 0) && (rounding_ == 2)) {
                --answer;
            }
            return FixedPoint(A_, B_, rounding_, answer);
        }

        FixedPoint operator/(FixedPoint& rhs) {
            if (rhs.number_ == 0) {
                return FixedPoint(0, 0, 0, 0);
            }

            return FixedPoint(A_, B_, rounding_, (number_ << B_) / rhs.number_);
        }


        void rounding() {
            if (rounding_ == 0) {
                toward_zero();
            }
        }

        bool getNegSign() {
            return neg_sign_;
        }

        long long getIntPart() {
            return int_part_;
        }

        long long getFractionalPart() {
            return fractional_part_;
        }

        int getDigit3() {
            return digit3_;
        }

        int getA() {
            return A_;
        }

    private:
        void toward_zero() {
            if (neg_sign_ && fractional_part_ != 0) {
                int_part_ += 1;
                fractional_part_ = (static_cast<int64_t>(1) << B_) - fractional_part_;
            }
            fractional_part_ *= 1000;

            digit3_ = static_cast<int>(fractional_part_ >> B_);
        }

        int A_;
        int B_;
        char rounding_;
        long long number_;
        long long  int_part_;
        long long fractional_part_;
        int digit3_;
        bool neg_sign_ = false;
};

std::ostream& operator<<(std::ostream& stream, FixedPoint num) {
    if (num.getA() == 0) {
        std::cerr << "division by zero";
        std::exit(0);
    }
    num.rounding();
    if (num.getNegSign() && (num.getIntPart()  == 0 && num.getDigit3() != 0)) {
        std::cout << "-";
    }
    std::cout << num.getIntPart() << ".";
    if (num.getDigit3() < 10) {
        std::cout << "00";
    }
    else if ((num.getDigit3() / 10 < 10)) {
        std::cout << "0";
    }
    std::cout << num.getDigit3();
    return stream;
}

bool isNumber0x(char* number) {
    if (number[0] != '0' || number[1] != 'x') {
        return false;
    }

    size_t size = std::strlen(number);
    for (int i = 2; i < size; ++i) {
        if ((!isdigit(number[i])) && (('a' > number[i] && 'A' > number[i]) || (number[i] > 'f' && number[i] > 'F'))) {
            return false;
        }
    }
    return true;
}

int ParseFloat(int argc, char** argv, ArgumentsFloat& parse_arguments) {
    if ((std::strlen(argv[1]) != 1) && (!((argv[1][0] == 'f') || (argv[1][0] == 'h')))) {
        std::cerr << "Error with type\n";
        std::exit(1);
    }
    parse_arguments.type = argv[1][0];

    if ((std::strlen(argv[2]) != 1) || (argv[2][0] != '0')) {
        std::cerr << "Error with rounding!\n";
        std::exit(1);
    }
    parse_arguments.rounding = argv[2][0] - '0';

    if (!isNumber0x(argv[3])) {
        std::cerr << "Error with number\n";
        std::exit(1);
    }
    parse_arguments.number_1 = std::strtoll(argv[3], 0, 16);

    if (argc == 6) {
        if ((std::strlen(argv[4]) != 1) || argv[4][0] != '+' && argv[4][0] != '-' && argv[4][0] != '*' && argv[4][0] != '/') {
            std::cerr << "Error with operation!\n";
            std::exit(1);
        }
        parse_arguments.operation = argv[4][0];

        if (!isNumber0x(argv[5])) {
            std::cerr << "Error with second number!\n";
            std::exit(1);
        }
        parse_arguments.number_2 = std::strtoll(argv[5], 0 ,16);

    }
    else if (argc != 4 && argc != 6) {
        std::cerr << "Error with argument!\n";
        std::exit(0);
    }
    return 1;
}


int ParseFixed(int argc, char** argv, ArgumentsFixed& parse_arguments) {
    const char* pos = std::strchr(argv[1], '.');
    if (pos == 0) {
        std::cerr << "Error with format!\n";
        std::exit(0);
    }
    int tmp = 0;
    size_t size = std::strlen(argv[1]);
    for (int i = 0; i < size; ++i) {
        if (argv[1][i] != '.') {
            tmp *= 10;
            tmp += argv[1][i] - '0'; 
        }
        else {
            parse_arguments.A = tmp;
            tmp = 0;
        }
    }
    parse_arguments.B = tmp;
    if (parse_arguments.A == 0) {
        std::cerr << "Error with format!\n";
        std::exit(0);
    }

    if ((std::strlen(argv[2]) != 1) || (argv[2][0] != '0')) {
        std::cerr << "Error with rounding!\n";
        std::exit(0);
    }
    parse_arguments.rounding = argv[2][0] - '0';
    if (!isNumber0x(argv[3])) {
        std::cerr << "Error with first number!\n";
        std::exit(0);
    }
    parse_arguments.number_1 = std::strtoll(argv[3], 0, 16);


    if (argc == 6) {
        if (std::strlen(argv[4]) != 1 || argv[4][0] != '+' && argv[4][0] != '-' && argv[4][0] != '*' && argv[4][0] != '/') {
            std::cerr << "Error with operation!\n";
            std::exit(0);
        }
        parse_arguments.operation = argv[4][0];

        if (!isNumber0x(argv[5])) {
            std::cerr << "Error with second number!\n";
            std::exit(0);
        }
        parse_arguments.number_2 = std::strtoll(argv[5], 0 ,16);

    }
    else if (argc != 4 && argc != 6) {
        std::cerr << "Error with argument!\n";
        std::exit(0);
    }

    return 1;
}


int main(int argc, char** argv) {
    if (std::strlen(argv[1]) > 1) {
        ArgumentsFixed parse_arguments;
        ParseFixed(argc, argv, parse_arguments);

        FixedPoint number1(parse_arguments.A, parse_arguments.B, parse_arguments.rounding, parse_arguments.number_1);   

        if (parse_arguments.operation == 0) {
            std::cout << number1;
            return 0;
        }

        FixedPoint number2(parse_arguments.A, parse_arguments.B, parse_arguments.rounding, parse_arguments.number_2);
        
        if (parse_arguments.operation == '+') {
            std::cout << number1 + number2;
        }
        else if (parse_arguments.operation == '-') {
            std::cout << number1 - number2;
        }
        else if (parse_arguments.operation == '*') {
            std::cout << number1 * number2;
        }
        else if (parse_arguments.operation == '/') {
            std::cout << number1 / number2;
        }
    }
    else {
        ArgumentsFloat parse_arguments;
        ParseFloat(argc, argv, parse_arguments);

        if (parse_arguments.type == 'h') {

            FloatPoint<10, 5, 15, -15, 16,32256, 31744> number1(parse_arguments.number_1, parse_arguments.rounding);   

            if (parse_arguments.operation == 0) {
                std::cout << number1;
                return 0;
            }

            FloatPoint<10, 5, 15, -15, 16,32256, 31744> number2(parse_arguments.number_2, parse_arguments.rounding);     
            if (parse_arguments.operation == '+') {
                std::cout << number1 + number2;
            }
            else if (parse_arguments.operation == '-') {
                std::cout << number1 - number2;
            }
            else if (parse_arguments.operation == '*') {
                std::cout << number1 * number2;
            }
            else if (parse_arguments.operation == '/') {
                std::cout << number1 / number2;
            }
        }
        else if (parse_arguments.type == 'f') {
            FloatPoint<23, 8, 127, -127, 128, 2143289344, 2139095040> number1(parse_arguments.number_1, parse_arguments.rounding);   

            if (parse_arguments.operation == 0) {
                std::cout << number1;
                return 0;
            }

            FloatPoint<23, 8, 127, -127, 128, 2143289344, 2139095040> number2(parse_arguments.number_2, parse_arguments.rounding);     
            if (parse_arguments.operation == '+') {
                std::cout << number1 + number2;
            }
            else if (parse_arguments.operation == '-') {
                std::cout << number1 - number2;
            }
            else if (parse_arguments.operation == '*') {
                std::cout << number1 * number2;
            }
            else if (parse_arguments.operation == '/') {
                std::cout << number1 / number2;
            }
        }
    }
    return 0;
}
