#pragma once

#include <vector>
#include <ostream>
#include <unordered_set>
#include <utility>

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

template <class A, class B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B>& p) {
    out << "(";

    out << p.first << "," << p.second;

    out << ")";
    return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& vs) {
    out << "{";

    bool first = true;
    for (const auto& v : vs) {
        if (!first) out << ",";
        first = false;
        out << v;
    }

    out << "}";
    return out;
}

