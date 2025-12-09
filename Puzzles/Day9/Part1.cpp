#include <fstream>
#include <iostream>
#include <vector>

#include "../../Libs/Heap.hpp"

struct Point {
    size_t x{};
    size_t y{};

    Point operator-(const Point& rhs) const {
        // get absolute difference in side lengths (no negatives)
        return Point{(x > rhs.x ? x - rhs.x : rhs.x - x) + 1, (y > rhs.y ? y - rhs.y : rhs.y - y) + 1};
    }

    // specialization to get area of rectangle defined by side lengths
    size_t operator()() const {
        return x * y;
    }
};

bool LargerArea(const std::pair<const Point*, const Point*>& a, const std::pair<const Point*, const Point*>& b) {
    return (*a.first - *a.second)() > (*b.first - *b.second)();
}

std::istream& operator>>(std::istream& in, Point& p);

int main() {
    std::ifstream input("input.txt");

    std::vector<Point> tiles{};
    Point p;

    while (input >> p) {
        tiles.emplace_back(p);
    }

    Heap<std::pair<const Point*, const Point*>, LargerArea> maxAreaHeap{};

    for (size_t i = 0; i < tiles.size() - 1; ++i) {
        for (size_t j = i + 1; j < tiles.size(); ++j) {
            maxAreaHeap.Enqueue({&tiles.at(i), &tiles.at(j)});
        }
    }

    auto tilePair = std::move(maxAreaHeap.Dequeue());

    std::cout << (*tilePair.first - *tilePair.second)() << std::endl;

    return 0;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.x;
    in.ignore();  // ','
    return in >> p.y;
}
