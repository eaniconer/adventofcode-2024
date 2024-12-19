#include "file_reader.hpp"

#include "str/split.hpp"

#include <iostream>
#include <unordered_map>

namespace {

size_t count_all_possible(std::string_view design, const std::vector<std::string_view>& towels, std::unordered_map<std::string_view, size_t>& cache) {
    if (cache.contains(design))
        return cache[design];

    if (design.empty()) return 1;

    size_t count = 0;
    for (auto towel : towels) {
        if (design.starts_with(towel)) {
            count += count_all_possible(design.substr(towel.size()), towels, cache);
        }
    }

    cache[design] = count;
    return count;
}

}

int main() {
    auto lines = utils::readLines("input.txt");

    auto towels = utils::split(lines[0], ", ");
    std::unordered_map<std::string_view, size_t> cache;

    size_t all_possible_count = 0;
    for (size_t i = 2; i < lines.size(); ++i) {
        std::cout << i << std::endl;
        all_possible_count += count_all_possible(lines[i], towels, cache);
    }

    std::cout << "All Possible design count: " << all_possible_count << std::endl;

    return 0;
}