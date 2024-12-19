#include "file_reader.hpp"

#include "str/split.hpp"

#include <iostream>

namespace {

bool is_possible(std::string_view design, const std::vector<std::string_view>& towels) {
    if (design.empty()) return true;

    for (auto towel : towels) {
        if (design.starts_with(towel)) {
            if (is_possible(design.substr(towel.size()), towels))
                return true;
        }
    }
    return false;
}

}

int main() {
    auto lines = utils::readLines("input.txt");

    auto towels = utils::split(lines[0], ", ");

    size_t possible_count = 0;
    for (size_t i = 2; i < lines.size(); ++i) {
        std::cout << i << std::endl;
        if (is_possible(lines[i], towels))
            possible_count++;
    }

    std::cout << "Possible design count: " << possible_count << std::endl;

    return 0;
}