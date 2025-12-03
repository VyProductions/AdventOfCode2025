#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <string>
#include <utility>

class BigInt {
public:
    BigInt(size_t value = 0) {
        std::string tmp = std::to_string(value);
        m_str.resize(tmp.size());

        size_t i = tmp.size() - 1;
        size_t j = 0;

        while (i < tmp.size()) {
            m_str.at(j++) = tmp.at(i--);
        }
    }

    BigInt& operator+=(const BigInt& rhs) {
        size_t idx = 0;
        size_t min_size = m_str.size() < rhs.m_str.size() ? m_str.size() : rhs.m_str.size();
        size_t max_size = m_str.size() + rhs.m_str.size() - min_size;
        bool this_larger = max_size == m_str.size();

        short carry = 0;

        std::string new_str(max_size, '\0');

        while (idx < min_size) {
            short res = m_str.at(idx) - '0' + rhs.m_str.at(idx) - '0' + carry;
            carry = res / 10;
            new_str.at(idx) = res % 10 + '0';

            ++idx;
        }

        const std::string& ref = this_larger ? m_str : rhs.m_str;

        while (idx < max_size) {
            short res = ref.at(idx) - '0' + carry;
            carry = res / 10;
            new_str.at(idx) = res % 10 + '0';

            ++idx;
        }

        if (carry > 0) {
            new_str.push_back(carry + '0');
        }

        m_str = std::move(new_str);

        return *this;
    }

    friend
    std::ostream& operator<<(std::ostream& out, const BigInt& big) {
        for (auto it = big.m_str.rbegin() ; it != big.m_str.rend(); ++it) {
            out << *it;
        }

        return out;
    }

private:
    std::string m_str;
};

#endif
