#include "file_reader.hpp"

#include "str/split.hpp"
#include "io/ostream_support.hpp"

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

    using Clique = std::vector<Computer>;
    std::vector<Clique> cliques;

    // build 2-cliques
    for (const auto& [c1, neibs] : graph) {
        for (auto c2 : neibs) {
            if (!(c1 < c2)) continue;
            cliques.push_back({c1, c2});
        }
    }
    std::cout << "2-cliques count: " << cliques.size() << std::endl;

    std::vector<Clique> up_cliques;
    while (!cliques.empty()) {
        up_cliques.clear();

        for (const auto& [c, _] : graph) {
            for (const auto& clique : cliques) {
                bool can_extend = true;

                for (Computer cc : clique) {
                    if (!(cc < c)) {
                        can_extend = false;
                        break;
                    }
                    if (!graph[cc].contains(c)) {
                        can_extend = false;
                        break;
                    }
                }

                if (can_extend) {
                    up_cliques.push_back(clique);
                    up_cliques.back().push_back(c);
                }
            }
        }

        if (!up_cliques.empty()) {
            std::cout << up_cliques[0].size() << "-cliques count: " << up_cliques.size() << std::endl;

            if (up_cliques.size() == 1) {
                std::cout << "\t" << up_cliques[0] << std::endl;
            }
        }



        std::swap(up_cliques, cliques);
    }

/*
Output for my input:
2-cliques count: 3380
3-cliques count: 11011
4-cliques count: 26455
5-cliques count: 45045
6-cliques count: 55770
7-cliques count: 50622
8-cliques count: 33462
9-cliques count: 15730
10-cliques count: 5005
11-cliques count: 975
12-cliques count: 91
13-cliques count: 1
*/

    return 0;
}