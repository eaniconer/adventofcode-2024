#pragma once

#include <charconv>
#include <string_view>

namespace utils {

template <class T>
T parse(std::string_view s) {
    T t{};
    std::from_chars(s.begin(), s.end(), t);
    return t;
}

}