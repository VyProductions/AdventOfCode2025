#include <fstream>
#include <iostream>
#include <unordered_map>

int main() {
    std::ifstream input("input.txt");

    std::unordered_map<size_t, bool> lasers{};
    
    unsigned num_splits = 0;

    // get starting laser
    size_t idx = 0;
    while (input.peek() != 'S') {
        input.ignore();
        ++idx;
    }
    input.ignore(400, '\n');

    // ignore gap line
    input.ignore(400, '\n');

    // initialize laser
    lasers[idx] = true;

    while (input.peek() != EOF) {
        idx = 0;

        while (input.peek() != '\n') {
            if (input.get() == '^' && lasers[idx]) {
                lasers[idx - 1] = true;
                lasers[idx + 1] = true;
                lasers[idx] = false;

                ++num_splits;
            }

            ++idx;
        }

        // ignore gap line
        input.ignore(400, '\n');
    }

    std::cout << num_splits << std::endl;

    return 0;
}
