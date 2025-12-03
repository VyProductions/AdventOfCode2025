#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Range {
    size_t lo{};
    size_t hi{};

    std::pair<std::string, std::string> GetFirstAndLastPrefix() {
        std::pair<std::string, std::string> cands{"0", "0"};

        std::string lstr = std::to_string(lo);

        if (lstr.size() % 2 == 0) {  // first candidate ID is either the left half duplicated or left half + 1 duplicated
            std::string lhs = lstr.substr(0, lstr.size() / 2);
            size_t lhsull = std::stoull(lhs);
            size_t cand = std::stoull(lhs + lhs);
            size_t cand2 = std::stoull(std::to_string(lhsull + 1) + std::to_string(lhsull + 1));

            if (cand < lo) {
                cand = cand2;
            }

            if (cand > hi) {
                cand = 0;
            }

            cands.first = std::to_string(cand);
        } else {  // first candidate ID is 10 ^ (len(lstr) / 2) duplicated
            std::string one_zero = "1" + std::string(lstr.size() / 2, '0');
            size_t cand = std::stoull(one_zero + one_zero);

            if (cand > hi) {
                cand = 0;
            }

            cands.first = std::to_string(cand);
        }

        if (cands.first != "0") {
            cands.first = cands.first.substr(0, cands.first.size() / 2);
        }

        std::string hstr = std::to_string(hi);

        if (hstr.size() % 2 == 0) {  // last candidate ID is either the LHS duplicated or LHS - 1 duplicated
            std::string lhs = hstr.substr(0, hstr.size() / 2);
            size_t lhsull = std::stoull(lhs);
            size_t cand = std::stoull(lhs + lhs);
            size_t cand2 = std::stoull(std::to_string(lhsull - 1) + std::to_string(lhsull - 1));

            if (cand > hi) {
                cand = cand2;
            }

            if (cand < lo) {
                cand = 0;
            }

            cands.second = std::to_string(cand);
        } else {
            size_t cand = std::stoull(std::string(hstr.size() - 1, '9'));

            if (cand < lo) {
                cand = 0;
            }

            cands.second = std::to_string(cand);
        }

        if (cands.second != "0") {
            cands.second = cands.second.substr(0, cands.second.size() / 2);
        }

        return cands;
    }
};

std::istream& operator>>(std::istream& in, Range& range);
std::istream& operator>>(std::istream& in, std::vector<Range>& ranges);
size_t GetInvalidIDSum(const std::pair<size_t, size_t>& endpoints);

int main() {
    std::ifstream input("input.txt");

    std::vector<Range> ranges{};
    size_t total = 0;

    input >> ranges;

    for (Range& range : ranges) {
        std::pair<std::string, std::string> candidate_prefix_endpoints = range.GetFirstAndLastPrefix();

        if (candidate_prefix_endpoints.first != "0") {
            size_t subtotal = GetInvalidIDSum({std::stoull(candidate_prefix_endpoints.first), std::stoull(candidate_prefix_endpoints.second)});

            total += subtotal;
        }
    }

    std::cout << total << std::endl;

    return 0;
}

std::istream& operator>>(std::istream& in, Range& range) {
    char dump;

    return in >> range.lo >> dump >> range.hi;
}

std::istream& operator>>(std::istream& in, std::vector<Range>& ranges) {
    Range new_range{};

    while (in >> new_range) {
        ranges.push_back(new_range);
        in.ignore();
    }

    return in;
}

size_t GetInvalidIDSum(const std::pair<size_t, size_t>& endpoints) {
    size_t sum = 0;

    size_t m_init = std::stoull("1" + std::string(std::to_string(endpoints.first).size() - 1, '0') + "1");
    size_t m_end  = std::stoull("1" + std::string(std::to_string(endpoints.second).size() - 1, '0') + "1");

    size_t first = endpoints.first;
    size_t last  = std::stoull(std::string(std::to_string(first).size(), '9'));

    if (last > endpoints.second) {
        last = endpoints.second;
    }

    for (size_t m = m_init ; m <= m_end; m = m * 10 - 9) {
        size_t num_elems = last - first + 1;
        size_t elem_sum = (num_elems * (first + last)) / 2;

        sum += m * elem_sum;

        first = last + 1;
        size_t new_last = last * 10 + 9;

        if (new_last > endpoints.second) {
            new_last = endpoints.second;
        }

        last = new_last;
    }

    return sum;
}
