#include <iostream>
#include <string>
#include <vector>
#include <bitset>

struct Arguments {
    int A;
    int B;
    int roundung;
    int number_1;
    char operation = 0;
    int number_2;
};

bool isNumber(char* str) {
    size_t size = strlen(str);
    for (int i = 0; i < size; ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool isNumber0x(char* number) {
    if (number[0] != '0' || number[1] != 'x') {
        return false;
    }

    size_t size = strlen(number);
    for (int i = 2; i < size; ++i) {
        if ((!isdigit(number[i])) && ('a' > number[i] || number[i] > 'f')) {
            return false;
        }
    }
    return true;
}


int Parse(int argc, char** argv, Arguments& parse_argumnets) {
    const char* pos = strchr(argv[1], '.');
    if (pos == 0) {
        std::cout << "Error with format!\n";
        return 0;
    }
    int tmp = 0;
    size_t size = strlen(argv[1]);
    for (int i = 0; i < size; ++i) {
        if (argv[1][i] != '.') {
            tmp *= 10;
            tmp += argv[1][i] - '0'; 
        }
        else {
            parse_argumnets.A = tmp;
            tmp = 0;
        }
    }
    parse_argumnets.B = tmp;
    if (parse_argumnets.A == 0) {
        std::cout << "Error with format!\n";
        return 0;
    }

    if ((strlen(argv[2]) != 1) || ('0' > argv[2][0]) || (argv[2][0] > '3')) {
        std::cout << "Error with rounding!\n";
        return 0;
    }
    parse_argumnets.roundung = argv[2][0] - '0';

    if (!isNumber0x(argv[3])) {
        std::cout << "Error with first number!\n";
        return 0;
    }
    parse_argumnets.number_1 = std::stoi(argv[3], 0 ,16);


    if (argc == 6) {
        if (strlen(argv[4]) != 1 || argv[4][0] != '+' && argv[4][0] != '-' && argv[4][0] != '*' && argv[4][0] != '/') {
            std::cout << "Error with operation!\n";
            return 0;
        }
        parse_argumnets.operation = argv[4][0];

        if (!isNumber0x(argv[5])) {
            std::cout << "Error with second number!\n";
            return 0;
        }
        parse_argumnets.number_2 = std::stoi(argv[5], 0 ,16);

    }
    else if (argc != 4 && argc != 6) {
        std::cout << "Error with argument!\n";
        return 0;
    }

    return 1;
}

void toward_zero(int int_part, int fractional_part, int A, int B) {
    if (int_part < 0 && fractional_part != 0) {
        int_part += 1;
        fractional_part = (1 << B) - fractional_part;
    }
    fractional_part *= 1000;

    int digit3 = (fractional_part >> B);
    std::cout << int_part << "." << digit3 << '\n';
}

void nearest_even(int numbern) {

}

void toward_infinity(int int_part, int fractional_part, int A, int B) {
    fractional_part *= 1000;

    int digit3 = (fractional_part >> B);
    if (fractional_part != 0 && int_part < 0) {
            ++int_part;
    }
    if (fractional_part % static_cast<int>(std::pow(2, B))) {
        digit3 += 1;
        if ((digit3 == 1000) && (int_part >= 0)) {
            ++int_part;
        }
    }
    if (int_part < 0) {
        digit3 = (1000 - digit3) % 1000;
    }

    std::cout << int_part << "." << digit3;
}

void toward_neg_infinity(int number) {

}



void printNumber(int number, int A, int B, int rounding) {
    int int_part = (number >> B) % static_cast<int>(std::pow(2, A));
    if (int_part >= std::pow(2, A-1)) {
        int_part -= std::pow(2, A);
    }
    // for (int i = 0; i < A - 1; ++i) {
    //     answer += std::pow(2, i) * ((number >> (B + i)) & 1);
    // }
    // answer -= std::pow(2, A - 1) * ((number >> (B + A - 1)) & 1);
    int64_t fractional_part = number % (1 << B);
    // int64_t fractional_part = (number % static_cast<int>(std::pow(2, B))) * 1000;
    // int digit3 = (fractional_part >> B);
    // int digit3 = 0;
    // for (int i = 0; i < 10; ++i) {
    //     digit3 += std::pow(2, i) * ((fractional_part >> (B + i)) & 1);
    // }
    
    if (rounding == 0) {
        //к нулю (toward_zero)
        toward_zero(int_part, fractional_part, A, B);
    }
    else if (rounding == 1) {
        //к ближайшему чётному (nearest_even)

    }
    else if (rounding == 2) {
        //к +∞ (toward_infinity)
        toward_infinity(int_part, fractional_part, A, B);
        
    }
    else if (rounding == 3) {
        //к -∞ (toward_neg_infinity)
        
    }
}

int main(int argc, char** argv) {
    Arguments parse_arguments;
    if (Parse(argc, argv, parse_arguments) == 0) {
        return 0;
    }
    

    if (parse_arguments.operation == 0) {
        printNumber(parse_arguments.number_1, parse_arguments.A, parse_arguments.B, parse_arguments.roundung);
    }
    else if (parse_arguments.operation == '+') {
        int answer = parse_arguments.number_1 + parse_arguments.number_2;
        printNumber(answer, parse_arguments.A, parse_arguments.B, parse_arguments.roundung);
    } 
    else if (parse_arguments.operation == '-') {
        int answer = parse_arguments.number_1 - parse_arguments.number_2;
        printNumber(answer, parse_arguments.A, parse_arguments.B, parse_arguments.roundung);
    }
    else if (parse_arguments.operation == '*') {
        int answer = (static_cast<int64_t>(parse_arguments.number_1) * parse_arguments.number_2) >> (parse_arguments.A + parse_arguments.B);
        printNumber(answer, parse_arguments.A, parse_arguments.B, parse_arguments.roundung);
    }
    return 0;
}