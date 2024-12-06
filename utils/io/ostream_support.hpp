#pragma once

#include <vector>
#include <ostream>

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vs) {
    out << "[";

    bool first = true;
    for (const auto& v : vs) {
        if (!first) out << ",";
        first = false;
        out << v;
    }

    out << "]";
    return out;
}