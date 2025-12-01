#include <iostream>
#include <fstream>

struct DialData {
    char direction = '\0';
    short distance = 0;

    short GetOffset() { return direction == 'L' ? -distance : distance; }
};

std::istream& operator>>(std::istream& in, DialData& data);

int main() {
    // input file
    std::ifstream input("input.txt");

    // puzzle data
    unsigned count_zeroes = 0;
    short dial_val = 50;
    DialData data{};

    // algo
    while (input >> data) {
        bool is_zero = dial_val == 0;
        short offset = data.GetOffset();

        dial_val += offset;

        while (dial_val < 0) {
            dial_val += 100;

            if (!is_zero) {
                ++count_zeroes;
            } else {
                is_zero = false;
            }
        }

        while (dial_val > 99) {
            dial_val -= 100;

            if (dial_val > 0) {
                ++count_zeroes;
            }
        }

        if (dial_val == 0) {
            ++count_zeroes;
        }
    }

    std::cout << count_zeroes << '\n';

    return 0;
}

std::istream& operator>>(std::istream& in, DialData& data) {
    data.direction = in.get();
    in >> data.distance;
    in.ignore();  // '\n'

    return in;
}
