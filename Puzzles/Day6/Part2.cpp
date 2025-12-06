#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void ReadProblems(std::ifstream& in, std::vector<std::vector<unsigned>>& operands, std::vector<char>& operators);

template <class T>
T Add(T a, T b) { return a + b; }

template <class T>
T Mult(T a, T b) { return a * b; }

int main() {
    std::ifstream input("input.txt");

    std::vector<std::vector<unsigned>> operands{};
    std::vector<char> operators{};

    size_t sum = 0;

    ReadProblems(input, operands, operators);

    for (int i = 0; i < operators.size(); ++i) {
        auto op_func = (operators.at(i) == '+' ? Add<size_t> : Mult<size_t>);

        size_t res = operands.at(i).at(0);

        for (int j = 1; j < operands.at(i).size(); ++j) {
            res = op_func(res, operands.at(i).at(j));
        }

        sum += res;
    }

    std::cout << sum << std::endl;

    return 0;
}

void ReadProblems(std::ifstream& in, std::vector<std::vector<unsigned>>& operands, std::vector<char>& operators) {
    std::vector<std::string> operand_content{};
    std::vector<std::pair<size_t, size_t>> data_boundaries;

    // read in all operand lines
    std::string line;
    while (in.peek() != '+' && in.peek() != '*' && std::getline(in, line, '\n')) {
        operand_content.emplace_back(line);
    }

    // read in operator line character-by-character and determine data group boundary indices
    size_t start_idx = 0;
    size_t end_idx   = 0;

    while (in.peek() != '\n') {
        char ch = in.get();

        if (ch == '+' || ch == '*') {
            operators.push_back(ch);

            if (end_idx > start_idx) {
                data_boundaries.emplace_back(std::pair<size_t, size_t>{start_idx, end_idx - 2});
                start_idx = end_idx;
            }
        }

        ++end_idx;
    }

    data_boundaries.emplace_back(std::pair<size_t, size_t>{start_idx, end_idx - 1});

    // form vectors of operands
    for (size_t i = 0; i < data_boundaries.size(); ++i) {
        size_t start_idx = data_boundaries.at(i).first;
        size_t end_idx = data_boundaries.at(i).second;
        
        std::vector<unsigned> operand_form(end_idx - start_idx + 1, 0);

        for (size_t k = 0; k < operand_content.size(); ++k) {
            for (size_t j = start_idx; j <= end_idx; ++j) {
                char ch = operand_content.at(k).at(j);
                
                if (ch >= '0' && ch <= '9') {
                    operand_form.at(j - start_idx) = operand_form.at(j - start_idx) * 10 + ch - '0';
                }
            }
        }

        operands.emplace_back(operand_form);
    }
}
