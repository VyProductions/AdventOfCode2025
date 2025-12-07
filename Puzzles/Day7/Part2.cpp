#include <fstream>
#include <iostream>
#include <unordered_map>

int main() {
    std::ifstream input("input.txt");

    std::unordered_map<size_t, bool> lasers{};

    std::unordered_map<
        size_t,  // row
        std::unordered_map<
            size_t,  // ending positions of all unique particles
            size_t   // number of particles that pass through this point
        >
    > paths_at_row;
    
    size_t num_universes = 0;

    // get starting laser
    size_t idx = 0;

    size_t row = 0;
    while (input.peek() != 'S') {
        input.ignore();
        ++idx;
    }

    size_t max_idx = idx;

    while (input.peek() != '\n') {
        input.ignore();
        ++max_idx;
    }
    input.ignore();  // '\n'

    // ignore gap line
    input.ignore(400, '\n');

    // initialize laser
    lasers[idx] = true;
    paths_at_row[row][idx]++;

    while (input.peek() != EOF) {
        idx = 0;

        while (input.peek() != '\n') {
            if (input.get() == '^' && lasers[idx]) {
                lasers[idx - 1] = true;
                lasers[idx + 1] = true;
                lasers[idx] = false;

                // add new end point to existing paths that ended at this index
                size_t count = paths_at_row[row][idx];

                paths_at_row[row + 1][idx - 1] += count;
                paths_at_row[row + 1][idx + 1] += count;
            } else {
                // extend previous paths that ended at this index with the same end position
                paths_at_row[row + 1][idx] += paths_at_row[row][idx];
            }

            ++idx;
        }
        input.ignore();  // '\n'

        // ignore gap line
        input.ignore(400, '\n');

        ++row;
    }

    for (size_t c = 0; c < max_idx; ++c) {
        num_universes += paths_at_row[row][c];
    }

    std::cout << num_universes << std::endl;

    return 0;
}
