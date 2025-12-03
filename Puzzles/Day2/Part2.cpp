#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Range {
    size_t lo{};
    size_t hi{};

    std::pair<std::string, std::string> GetFirstAndLastPrefix(unsigned short divisions) {
        std::pair<std::string, std::string> cands{"0", "0"};

        std::string lstr = std::to_string(lo);

        if (lstr.size() % divisions == 0) {
            std::string lhs = lstr.substr(0, lstr.size() / divisions);
            size_t lhsull = std::stoull(lhs);

            std::string cand_str = lhs;
            std::string cand2_str = std::to_string(lhsull + 1);

            for (unsigned short i = 1; i < divisions; ++i) {
                cand_str += lhs;
                cand2_str += std::to_string(lhsull + 1);
            }
            
            size_t cand = std::stoull(cand_str);
            size_t cand2 = std::stoull(cand2_str);

            if (cand < lo) {
                cand = cand2;
            }

            if (cand > hi) {
                cand = 0;
            }

            cands.first = std::to_string(cand);
        } else {
            std::string one_zero = "1" + std::string(lstr.size() / divisions, '0');

            std::string cand_str = one_zero;

            for (unsigned short i = 1; i < divisions; ++i) {
                cand_str += one_zero;
            }

            size_t cand = std::stoull(cand_str);

            if (cand > hi) {
                cand = 0;
            }

            cands.first = std::to_string(cand);
        }

        if (cands.first != "0") {
            cands.first = cands.first.substr(0, cands.first.size() / divisions);
        }

        std::string hstr = std::to_string(hi);

        if (hstr.size() % divisions == 0) {
            std::string lhs = hstr.substr(0, hstr.size() / divisions);
            size_t lhsull = std::stoull(lhs);

            std::string cand_str = lhs;
            std::string cand2_str = std::to_string(lhsull - 1);
            
            for (unsigned short i = 1; i < divisions; ++i) {
                cand_str += lhs;
                cand2_str += std::to_string(lhsull - 1);
            }
            
            size_t cand = std::stoull(cand_str);
            size_t cand2 = std::stoull(cand2_str);

            if (cand > hi) {
                cand = cand2;
            }

            if (cand < lo) {
                cand = 0;
            }

            cands.second = std::to_string(cand);
        } else {
            size_t cand = std::stoull(std::string((hstr.size() / divisions) * divisions, '9'));

            if (cand < lo) {
                cand = 0;
            }

            cands.second = std::to_string(cand);
        }

        if (cands.second != "0") {
            cands.second = cands.second.substr(0, cands.second.size() / divisions);
        }

        return cands;
    }
};

std::istream& operator>>(std::istream& in, Range& range);
std::istream& operator>>(std::istream& in, std::vector<Range>& ranges);
size_t GetMult(unsigned short divisions, unsigned short substr_len);
size_t GetInvalidIDSum(
    const std::pair<size_t, size_t>& endpoints,
    unsigned short divisions,
    std::unordered_map<size_t, bool>& invalid_id_map
);

int main() {
    std::ifstream input("input.txt");

    std::vector<Range> ranges{};
    size_t total = 0;

    input >> ranges;

    for (Range& range : ranges) {
        unsigned max_div = (unsigned)std::to_string(range.hi).size();

        std::unordered_map<size_t, bool> invalid_id_map{};

        for (unsigned short division = 2; division <= max_div; ++division) {
            std::pair<std::string, std::string> candidate_prefix_endpoints = range.GetFirstAndLastPrefix(division);

            if (candidate_prefix_endpoints.first != "0") {
                size_t subtotal = GetInvalidIDSum(
                    {std::stoull(candidate_prefix_endpoints.first), std::stoull(candidate_prefix_endpoints.second)},
                    division,
                    invalid_id_map
                );

                total += subtotal;
            }
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

size_t GetMult(unsigned short divisions, unsigned short substr_len) {
    static std::unordered_map<
        unsigned short,
        std::unordered_map<
            unsigned short,
            size_t
        >
    > memo{};

    if (divisions == 0) {
        return 0;
    }

    if (memo.count(divisions) && memo.at(divisions).count(substr_len)) {
        return memo.at(divisions).at(substr_len);
    }

    return memo[divisions][substr_len] = std::pow(10, divisions * substr_len - substr_len) + GetMult(divisions - 1, substr_len);
}

size_t GetInvalidIDSum(
    const std::pair<size_t, size_t>& endpoints,
    unsigned short divisions,
    std::unordered_map<size_t, bool>& invalid_id_map
) {
    size_t sum = 0;

    size_t m_init = GetMult(divisions, std::to_string(endpoints.first).size());
    size_t m_end  = GetMult(divisions, std::to_string(endpoints.second).size());

    size_t first = endpoints.first;
    size_t last  = std::stoull(std::string(std::to_string(first).size(), '9'));

    if (last > endpoints.second) {
        last = endpoints.second;
    }

    unsigned short curr_len = std::to_string(endpoints.first).size();
    for (size_t m = m_init ; m <= m_end; m = GetMult(divisions, ++curr_len)) {
        size_t elem_sum = 0;

        for (size_t elem = first; elem <= last; ++elem) {
            std::string elem_str = std::to_string(elem);

            for (int i = 1; i < divisions; ++i) {
                elem_str += std::to_string(elem);
            }

            if (!invalid_id_map.count(std::stoull(elem_str))) {
                invalid_id_map[std::stoull(elem_str)] = true;
                elem_sum += elem;
            }
        }

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
