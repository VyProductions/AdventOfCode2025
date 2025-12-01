#include <iostream>
#include <fstream>

struct DialData {
    char direction = '\0';
    short distance = 0;
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
        dial_val += data.distance;

        while (dial_val < 0) dial_val += 100;
        while (dial_val > 99) dial_val -= 100;

        count_zeroes += dial_val == 0;
    }

    std::cout << count_zeroes << '\n';

    return 0;
}

std::istream& operator>>(std::istream& in, DialData& data) {
    data.direction = in.get();
    in >> data.distance;
    in.ignore();  // '\n'
    
    if (data.direction == 'L') data.distance *= -1;

    return in;
}