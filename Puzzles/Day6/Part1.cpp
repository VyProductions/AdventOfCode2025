#include <fstream>
#include <iostream>
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
    while (in.peek() != '+' && in.peek() != '*') {
        unsigned operand;
        size_t col = 0;

        while (in.peek() != '\n') {
            in >> operand;

            if (operands.size() == col) {
                operands.emplace_back(std::vector<unsigned>{operand});
            } else {
                operands.at(col).push_back(operand);
            }

            while (!(in.peek() >= '0' && in.peek() <= '9') && in.peek() != '\n') {
                in.ignore();
            }

            ++col;
        }
        in.ignore();  // '\n'
    }

    char op;

    while (in >> op) {
        operators.push_back(op);
    }
}
