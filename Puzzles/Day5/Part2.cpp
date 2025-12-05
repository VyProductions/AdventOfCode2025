#include <fstream>
#include <iostream>
#include <vector>

struct IngredientDB {
    std::vector<std::pair<size_t, size_t>> id_ranges{};
    std::vector<size_t> valid_ids{};
};

std::istream& operator>>(std::istream& in, IngredientDB& idb);

int main() {
    std::ifstream input("input.txt");

    IngredientDB idb{};
    size_t fresh_count = 0;

    input >> idb;

    for (auto& range : idb.id_ranges) {
        fresh_count += range.second - range.first + 1;
    }

    std::cout << fresh_count << std::endl;

    return 0;
}

std::istream& operator>>(std::istream& in, IngredientDB& idb) {
    while (in.peek() != '\n') {
        size_t lo;
        size_t hi;

        in >> lo;
        in.ignore();  // '-'
        in >> hi;
        in.ignore();  // '\n'

        auto it = idb.id_ranges.begin();

        while (it != idb.id_ranges.end() && it->first < lo) {
            ++it;
        }

        idb.id_ranges.insert(it, {lo, hi});
    }

    auto it = idb.id_ranges.begin();
    auto it2 = it;

    while (++it2 != idb.id_ranges.end()) {
        if (it->first <= it2->first && it->second >= it2->first && it->second <= it2->second) {
            // left edge overlap
            it2->first = it->first;
            it = idb.id_ranges.erase(it);
        } else if (it->first <= it2->first && it->second >= it2->second) {
            // entire range overlap
            it2->first = it->first;
            it2->second = it->second;
            it = idb.id_ranges.erase(it);
        } else if (it->first <= it2->second && it->second >= it2->second) {
            // right edge overlap
            it2->second = it->second;
            it = idb.id_ranges.erase(it);
        } else {
            ++it;
        }

        it2 = it;
    }

    // blank line
    in.ignore();  // '\n'

    size_t id;
    while (in >> id) {
        idb.valid_ids.push_back(id);
    }

    return in;
}
