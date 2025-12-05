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

    for (size_t id : idb.valid_ids) {
        bool found_range = false;

        for (size_t range_idx = 0; !found_range && range_idx < idb.id_ranges.size(); ++range_idx) {
            found_range = id >= idb.id_ranges.at(range_idx).first && id <= idb.id_ranges.at(range_idx).second;
        }

        fresh_count += found_range;
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

        idb.id_ranges.push_back({lo, hi});
    }

    // blank line
    in.ignore();  // '\n'

    size_t id;
    while (in >> id) {
        idb.valid_ids.push_back(id);
    }

    return in;
}
