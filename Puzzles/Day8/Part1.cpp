#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../../Libs/Heap.hpp"

struct Point {
    size_t x{};
    size_t y{};
    size_t z{};

    size_t circuit_id{};

    double Magnitude() const { 
        return std::abs(std::sqrt(x * x + y * y + z * z));
    }

    Point operator-(const Point& rhs) const {
        return Point{x - rhs.x, y - rhs.y, z - rhs.z};
    }
};

std::istream& operator>>(std::istream& in, Point& p);

bool LessPointDistance(const std::pair<const Point*, const Point*>& a, const std::pair<const Point*, const Point*>& b) {
    return (*a.second - *a.first).Magnitude() < (*b.second - *b.first).Magnitude();
}

bool LargestCircuit(const std::vector<size_t>& a, const std::vector<size_t>& b) {
    return a.size() > b.size();
}

int main() {
    std::ifstream input("input.txt");

    Heap<int, Heap<int>::CompLess> minHeap{};

    std::unordered_map<size_t, Point> boxes{};
    Point p;

    Heap<std::pair<const Point*, const Point*>, LessPointDistance> minBoxDistHeap{};

    std::unordered_map<
        size_t,  // circuit owner ID
        std::vector<size_t>  // all ID's in circuit
    > circuits{};

    while (input >> p) {
        p.circuit_id = boxes.size();
        boxes[boxes.size()] = p;
        circuits[p.circuit_id] = {p.circuit_id};
    }

    // create and enqueue all possible connections
    for (size_t i = 0; i < boxes.size() - 1; ++i) {
        for (size_t j = i + 1; j < boxes.size(); ++j) {
            minBoxDistHeap.Enqueue({&boxes[i], &boxes[j]});
        }
    }

    for (int i = 0 ; i < 1000; ++i) {
        std::pair<const Point*, const Point*> connection = minBoxDistHeap.Dequeue();

        size_t aCircuitID = connection.first->circuit_id;
        size_t bCircuitID = connection.second->circuit_id;

        // if the boxes are in different circuits, we need to combine them into one larger circuit
        if (aCircuitID != bCircuitID) {
            std::vector<size_t>& aCircuit = circuits[aCircuitID];
            std::vector<size_t>& bCircuit = circuits[bCircuitID];

            if (aCircuit.size() >= bCircuit.size()) {
                for (size_t id : bCircuit) {
                    boxes[id].circuit_id = aCircuitID;
                    aCircuit.push_back(id);
                }
                circuits.erase(bCircuitID);
            } else {
                for (size_t id : aCircuit) {
                    boxes[id].circuit_id = bCircuitID;
                    bCircuit.push_back(id);
                }
                circuits.erase(aCircuitID);
            }
        }
    }

    Heap<std::vector<size_t>, LargestCircuit> maxCircuitSizeHeap{};

    for (size_t i = 0; i < boxes.size(); ++i) {
        if (circuits.count(i)) {
            maxCircuitSizeHeap.Enqueue(circuits[i]);
        }
    }

    size_t res = 1;

    for (int i = 0; i < 3; ++i) {
        std::vector<size_t> circuit = std::move(maxCircuitSizeHeap.Dequeue());
        res *= circuit.size();
    }

    std::cout << res << std::endl;

    return 0;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.x;
    in.ignore();  // ','
    in >> p.y;
    in.ignore();  // ','
    return in >> p.z;
}
