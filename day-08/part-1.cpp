#include "file_reader.hpp"
#include "ostream_support.hpp"
#include "hash/pair_support.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <utility>
#include <iostream>

namespace {

bool is_antenna(char ch) {
    return isalnum(ch);
}

using AntennaName = char;
using Coordinates = std::pair<size_t, size_t>;
using CoordinatesSet = std::unordered_set<Coordinates>;

}

int main() {
    auto field = utils::readLines("input.txt");

    std::unordered_map<AntennaName, CoordinatesSet> antennas;
    for (size_t r = 0; r < field.size(); ++r) {
        for (size_t c = 0; c < field[r].size(); ++c) {
            auto cell = field[r][c];
            if (is_antenna(cell)) {
                antennas[cell].emplace(std::make_pair(r, c));
            }
        }
    }

    // for (const auto& [k, v] : antennas) {
    //     std::cout << k << ": " << v << std::endl;
    // }

    CoordinatesSet unique_positions;

    for (const auto& [name, coords_set] : antennas) {
        for (auto it = coords_set.begin(); it != coords_set.end(); ++it) {
            auto r1 = it->first;
            auto c1 = it->second;
            for (auto jt = coords_set.begin(); jt != coords_set.end(); ++jt) {
                auto r2 = jt->first;
                auto c2 = jt->second;
                if (r1 == r2 && c1 == c2) continue;

                auto dr = r2 - r1;
                auto dc = c2 - c1;
                auto nr = r2 + dr;
                auto nc = c2 + dc;

                if (nr < field.size() && nc < field.at(0).size()) {
                    unique_positions.emplace(std::make_pair(nr, nc));
                }
            }
        }
    }

    std::cout << "Number of unique node positions: " << unique_positions.size() << std::endl;

    return 0;
}