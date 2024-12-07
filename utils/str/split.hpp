#pragma once 

#include <vector>
#include <string_view>

namespace utils {

std::vector<std::string_view> split(std::string_view s, std::string_view sep) {
    std::vector<std::string_view> pages;
    while (true) {
        auto pos = s.find(sep);
        pages.push_back(s.substr(0, pos));
        if (pos == s.npos)
            break;
        s.remove_prefix(pos + sep.size());
    }
    return pages;
}

std::vector<std::string_view> split(std::string_view s, char sep) {
    return split(s, std::string_view(std::addressof(sep), 1));
}
}