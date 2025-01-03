#include "file_reader.hpp"
#include "str/parse.hpp"

#include <algorithm>
#include <iostream>

namespace {
using Integer = size_t;

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

    Integer sum = 0;
    const size_t N_ITER = 2000;

    for (auto n : ns) {
        for (size_t iter = 0; iter < N_ITER; ++iter) {
            n = gen_next(n);
        }
        std::cout << ns[0] << " " << n << std::endl;
        sum += n;
    }

    std::cout << "the sum of the 2000th secret number generated by each buyer: " << sum << std::endl;

    return 0;
}