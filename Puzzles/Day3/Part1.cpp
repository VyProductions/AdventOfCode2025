#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream input("input.txt");

    std::string batteryBank = "";
    short result = 0;

    while (std::getline(input, batteryBank, '\n')) {
        size_t idx = 0;
        
        // find largest character from left to right
        int larg = 0;
        size_t l_idx = 0;

        while (idx < batteryBank.size() - 1) {
            if (batteryBank.at(idx) - '0' > larg) {
                larg = batteryBank.at(idx) - '0';
                l_idx = idx;
            }

            ++idx;
        }

        result += 10 * larg;

        // find largest character from right to l_idx + 1
        idx = batteryBank.size() - 1;

        larg = 0;
        size_t r_idx = 0;

        while (idx > l_idx) {
            if (batteryBank.at(idx) - '0' > larg) {
                larg = batteryBank.at(idx) - '0';
                r_idx = idx;
            }

            --idx;
        }

        result += larg;
    }

    std::cout << result << std::endl;

    return 0;
}
