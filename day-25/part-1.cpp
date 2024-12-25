#include "file_reader.hpp"

#include <vector>
#include <string_view>
#include <iostream>
#include <unordered_set>

namespace {

using Schema = std::vector<std::string_view>;

int schemaToID(const Schema& schema) {
    const auto h = schema.size();
    const auto w = schema.at(0).size();

    int ID = 0;
    for (size_t c = 0; c < w; ++c) {
        size_t counter = 0;
        for (size_t r = 0; r < h; ++r) {
            if (schema[r][c] == '#')
                counter++;
        }
        counter--;

        ID *= 10;
        ID += counter;
    }

    return ID;
}

bool is_overlapped(int key, int lock) {
    while (key && lock) {
        if (((key % 10) + (lock % 10)) > 5)
            return true;
        key /= 10;
        lock /= 10;
    }
    return false;
}

bool is_lock(const Schema& schema) {
    for (char ch : schema.at(0))
        if (ch != '#')
            return false;
    return true;
}

}

int main() {
    auto lines = utils::readLines("input.txt");

    Schema schema;

    std::vector<int> keys, locks;

    for (const auto& line : lines) {
        if (line.empty()) {
            (is_lock(schema) ? locks : keys).push_back(schemaToID(schema));
            schema.clear();
            continue;
        }
        schema.emplace_back(line);
    }
    (is_lock(schema) ? locks : keys).push_back(schemaToID(schema));

    std::cout << "keys.size() = " << keys.size() << std::endl;
    std::cout << "locks.size() = " << locks.size() << std::endl;

    size_t fit_counter = 0;
    for (auto key : keys) {
        for (auto lock : locks) {
            if (!is_overlapped(key, lock)) {
                fit_counter++;
            }
        }
    }

    std::cout << "Fit counter: " << fit_counter << std::endl;

    return 0;
}