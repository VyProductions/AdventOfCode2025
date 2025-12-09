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

bool RectIntersectsLoop(const std::pair<Point, Point>& candRect, const std::vector<Point>& tiles);

std::istream& operator>>(std::istream& in, Point& p);
std::ostream& operator<<(std::ostream& out, const Point& p);

int main() {
    std::ifstream input("input.txt");

    std::vector<Point> tiles{};
    Point p;

    while (input >> p) {
        tiles.emplace_back(p);
    }

    Heap<std::pair<const Point*, const Point*>, LargerArea> maxAreaHeap{};

    for (size_t i = 0; i < tiles.size() - 1; ++i) {
        const Point& aTile = tiles.at(i);

        for (size_t j = i + 1; j < tiles.size(); ++j) {
            const Point& bTile = tiles.at(j);
            
            // define all rectangles by their top left and bottom right corner for consistent processing
            std::pair<Point, Point> candRect {
                Point{aTile.x < bTile.x ? aTile.x : bTile.x, aTile.y < bTile.y ? aTile.y : bTile.y},
                Point{aTile.x > bTile.x ? aTile.x : bTile.x, aTile.y > bTile.y ? aTile.y : bTile.y},
            };

            if (!RectIntersectsLoop(candRect, tiles)) {
                maxAreaHeap.Enqueue({&tiles.at(i), &tiles.at(j)});
            }
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

std::ostream& operator<<(std::ostream& out, const Point& p) {
    return out << "(" << p.x << "," << p.y << ")";
}

// NOTE: Intersection logic from
// https://www.reddit.com/r/adventofcode/comments/1pi3hff/2025_day_9_part_2_a_simple_method_spoiler/
bool RectIntersectsLoop(const std::pair<Point, Point>& candRect, const std::vector<Point>& tiles) {
    bool hasIntersection = false;
    
    for (size_t i = 0; !hasIntersection && i < tiles.size(); ++i) {
        const Point& aPoint = tiles.at(i);
        const Point& bPoint = tiles.at((i + 1) % tiles.size());

        size_t minX = aPoint.x < bPoint.x ? aPoint.x : bPoint.x;
        size_t maxX = aPoint.x + bPoint.x - minX;
        size_t minY = aPoint.y < bPoint.y ? aPoint.y : bPoint.y;
        size_t maxY = aPoint.y + bPoint.y - minY;

        hasIntersection = candRect.first.x < maxX && candRect.second.x > minX && candRect.first.y < maxY && candRect.second.y > minY;
    }

    return hasIntersection;
}
