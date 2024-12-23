#include "file_reader.hpp"

#include "str/split.hpp"

#include <vector>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace {

using Computer = std::string_view;
using Computers = std::unordered_set<Computer>;

bool isGoodComputer(Computer a) {
    return a.starts_with("t");
}

bool isGoodTriple(Computer a, Computer b, Computer c) {
    return  isGoodComputer(a)
            || isGoodComputer(b)
            || isGoodComputer(c);
}

}

int main() {
    auto raw_pairs = utils::readLines("input.txt");

    std::unordered_map<Computer, Computers> graph;

    for (const auto& raw_pair : raw_pairs) {
        auto pair = utils::split(raw_pair, '-');
        auto c1 = pair.at(0);
        auto c2 = pair.at(1);
        graph[c1].insert(c2);
        graph[c2].insert(c1);
    }

    size_t counter = 0;
    for (const auto& [c1, neibs] : graph) {
        for (auto c2 : neibs) {
            if (!(c1 < c2)) continue;
            auto& c2_neibs = graph[c2];
            for (auto c3 : neibs) {
                if (!(c2 < c3)) continue;
                if (!c2_neibs.contains(c3)) continue;

                if (isGoodTriple(c1, c2, c3)) {
                    std::cout << c1 << " " << c2 << " " << c3 << std::endl;
                    counter++;
                }
            }
        }
    }

    std::cout << "Counter: " << counter << std::endl;


    return 0;
}