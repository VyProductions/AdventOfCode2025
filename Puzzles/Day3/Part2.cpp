#include <fstream>
#include <iostream>
#include <string>

#include "../../Libs/BigInt.hpp"

int main() {
    std::ifstream input("input.txt");

    std::string batteryBank = "";
    BigInt result = 0;

    while (std::getline(input, batteryBank, '\n')) {
        size_t f_idx = 0;
        size_t joltage = 0;

        for (size_t right_removal = 11; right_removal < 12; --right_removal) {
            short larg = 0;
            size_t l_idx = 0;

            size_t idx = batteryBank.size() - right_removal - 1;
            while (idx >= f_idx && idx < batteryBank.size()) {
                if (batteryBank.at(idx) - '0' >= larg) {
                    larg = batteryBank.at(idx) - '0';
                    l_idx = idx;
                }

                --idx;
            }

            f_idx = l_idx + 1;

            joltage *= 10;
            joltage += larg;
        }

        BigInt value(joltage);

        result += value;
    }

    std::cout << result << std::endl;

    return 0;
}
