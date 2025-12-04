#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <class Predicate>
unsigned CountNeighbors(const std::vector<std::string>& board, const std::pair<long, long>& coords, Predicate p);

int main() {
    std::ifstream input("input.txt");

    std::vector<std::string> board{};
    unsigned valid_count = 0;

    std::string line;
    while (std::getline(input, line, '\n')) {
        board.emplace_back(line);
    }

    for (long r = 0; r < board.size(); ++r) {
        for (long c = 0; c < board.at(0).size(); ++c) {
            if (board.at(r).at(c) == '@' && CountNeighbors(board, {r, c}, [](char a) { return a == '@' || a == 'x'; }) < 4) {
                board.at(r).at(c) = 'x';
                
                ++valid_count;
            }
        }
    }

    std::cout << valid_count << std::endl;

    return 0;
}

template <class Predicate>
unsigned CountNeighbors(const std::vector<std::string>& board, const std::pair<long, long>& coords, Predicate p) {
    unsigned count = 0;

    short offs_x[8] { -1,  0,  1, -1, 1, -1, 0, 1 };
    short offs_y[8] { -1, -1, -1,  0, 0,  1, 1, 1 };

    for (int i = 0; i < 8; ++i) {
        long res_x = coords.second + offs_x[i];
        long res_y = coords.first + offs_y[i];

        if (
            res_y >= 0 && res_y < board.size() &&
            res_x >= 0 && res_x < board.at(res_y).size()
        ) {
            count += p(board.at(res_y).at(res_x));
        }
    }

    return count;
}