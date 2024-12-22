#include "file_reader.hpp"
#include "str/parse.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>

namespace {
using Integer = int64_t;

const Integer PRUNE_COEF = 16777216;
Integer gen_next(Integer n) {
    n ^= (n * 64);
    n %= PRUNE_COEF;

    n ^= (n / 32);
    n %= PRUNE_COEF;

    n ^= (n * 2048);
    n %= PRUNE_COEF;

    return n;
}

}

int main() {
    auto lines = utils::readLines("input.txt");

    std::vector<Integer> ns;
    std::transform(lines.begin(), lines.end(), std::back_inserter(ns), utils::parse<Integer>);

    const size_t N_ITER = 2000;

    struct BestDiffsSearcher {
        std::unordered_map<Integer, Integer> all_diffs;
        Integer best_diffs;
    } searcher;

    int progress_tick = 0;
    for (auto n : ns) {
        Integer prev = 0;
        Integer diffs = 0;
        std::unordered_set<Integer> seen;
        for (size_t iter = 0; iter < N_ITER; ++iter) {
            n = gen_next(n);

            auto t = ((n % 10) - prev) + 10;
            prev = n % 10;

            if (iter > 0) {
                diffs *= 100;
                diffs += t;
            }

            diffs %= 1'00'00'00'00;
            if (iter < 3)
                continue;

            if (seen.contains(diffs))
                continue;

            seen.insert(diffs);

            searcher.all_diffs[diffs] += (n % 10);

            auto it = searcher.all_diffs.find(searcher.best_diffs);
            if (it == searcher.all_diffs.end() || it->second < searcher.all_diffs[diffs]) {
                searcher.best_diffs = diffs;
            }
        }

        float progress = (static_cast<float>(progress_tick) / ns.size()) * 100.f;
        progress_tick++;
        std::cout << "Progress: " << progress << std::endl;
    }

    auto n = searcher.all_diffs[searcher.best_diffs];
    std::cout << "the most bananas we can get: " << n << std::endl;

    return 0;
}