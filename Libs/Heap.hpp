#ifndef HEAP_HPP
#define HEAP_HPP

#include <initializer_list>
#include <utility>
#include <vector>

template <class T, bool (*Comp)(const T&, const T&) = nullptr>
class Heap {
public:
    using value_type = T;
    using size_type = size_t;

    static bool CompLess(const value_type& a, const value_type& b) { return a < b; }
    static bool CompGreater(const value_type& a, const value_type& b) { return a > b; }

    Heap() {
        m_data.clear();
    };

    Heap(const std::initializer_list<value_type>& ilist) {
        m_data.reserve(ilist.size());

        for (value_type& val : ilist) {
            m_data.push_back(val);
        }

        for (size_type i = m_data.size() - 1; i < m_data.size(); --i) {
            BubbleDown(i);
        }
    }

    void Enqueue(T value) {
        m_data.emplace_back(value);
        BubbleUp(m_data.size() - 1);
    }

    T Dequeue() {
        T value = m_data.front();

        // copy last element to root
        m_data.front() = m_data.back();

        // erase last element
        m_data.pop_back();

        // if at least one element, bubble down to restore heap ordering
        if (!m_data.empty()) {
            BubbleDown(0);
        }

        return value;
    }

    size_type size() const { return m_data.size(); }
    bool empty() const { return m_data.empty(); }
protected:
    static bool Compare(const T& a, const T& b) {
        if constexpr (Comp) {
            return Comp(a, b);
        } else {
            return CompLess(a, b);
        }
    }

    size_type Parent(size_type child) { return (child - 1) / 2; }
    size_type LIdx(size_type parent) { return parent * 2 + 1; }
    size_type RIdx(size_type parent) { return parent * 2 + 2; }
    
    void Swap(size_type aIdx, size_type bIdx) {
        value_type tmp = std::move(m_data.at(aIdx));
        m_data.at(aIdx) = std::move(m_data.at(bIdx));
        m_data.at(bIdx) = std::move(tmp);
    }

    void BubbleUp(size_type start) {
        // already at root
        if (start == 0) {
            return;
        }
        
        size_type next_idx = start;
        size_type parent = Parent(start);

        if (!Compare(m_data.at(parent), m_data.at(start))) {
            Swap(start, parent);
            next_idx = parent;
        }

        if (next_idx != start) {
            BubbleUp(next_idx);
        }
    }

    void BubbleDown(size_type start) {
        size_type next_idx = start;
        size_type lIdx = LIdx(start);
        
        // no L or R child
        if (lIdx >= m_data.size()) {
            return;
        }

        size_type rIdx = RIdx(start);

        // no R child
        if (rIdx >= m_data.size()) {
            if (!Compare(m_data.at(start), m_data.at(lIdx))) {
                Swap(start, lIdx);
                next_idx = lIdx;
            }
        // L and R child
        } else {
            next_idx = Compare(m_data.at(lIdx), m_data.at(rIdx)) ? lIdx : rIdx;

            if (!Compare(m_data.at(start), m_data.at(next_idx))) {
                Swap(start, next_idx);
            } else {
                next_idx = start;
            }
        }

        if (next_idx != start) {
            BubbleDown(next_idx);
        }
    }

    std::vector<value_type> m_data;
};

#endif
