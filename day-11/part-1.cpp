#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"
#include "hash/pair_support.hpp"

#include <cassert>
#include <iostream>
#include <unordered_map>

namespace {
using Integer = unsigned long long;

size_t ndigits(Integer n) {
    if (n == 0) return 1;

    size_t counter = 0;

    while (n) {
        counter++;
        n /= 10;
    }

    return counter;
}

std::vector<Integer> transform(Integer n) {
    std::vector<Integer> vs;

    if (n == 0) { 
        vs.push_back(1);
    } else if (auto nd = ndigits(n); nd % 2 == 0) {
        Integer the_second = 0;
        Integer mul = 1;
        while (nd) {
            the_second += mul * (n % 10);
            mul *= 10;
            n /= 10;
            nd -= 2;
        }
        vs.push_back(n);
        vs.push_back(the_second);
    } else {
        assert(n <= std::numeric_limits<Integer>::max() / 2024);
        vs.push_back(n * 2024);
    }

    return vs;
}

struct MemoTrasformer {
    size_t count_after_n_transforms(Integer n, size_t step_count) {
        if (step_count == 0)
            return 1;

        auto it = memory_.find(std::make_pair(n, step_count));
        if (it != memory_.end())
            return it->second;

        auto nxt_values = transform(n);
        size_t count = 0;
        for (auto value : nxt_values) {
            count += count_after_n_transforms(value, step_count - 1);
        }
        memory_[std::make_pair(n, step_count)] = count;
        return count;
    }
private:
    using Pair = std::pair<Integer, size_t>;
    std::unordered_map<Pair, size_t> memory_;
};

}

int main() { 
    {
        auto vs = transform(0);
        assert(vs.size() == 1);
        assert(vs[0] == 1);
    }
    {
        auto vs = transform(1);
        assert(vs.size() == 1);
        assert(vs[0] == 2024);
    }
    {
        auto vs = transform(10);
        assert(vs.size() == 2);
        assert(vs[0] == 1);
        assert(vs[1] == 0);
    }
    {
        auto vs = transform(1000);
        assert(vs.size() == 2);
        assert(vs[0] == 10);
        assert(vs[1] == 0);
    }
    {
        auto vs = transform(1212);
        assert(vs.size() == 2);
        assert(vs[0] == 12);
        assert(vs[1] == 12);
    }


    auto lines = utils::readLines("input.txt");
    assert(lines.size() == 1);

    auto raw_ns = utils::split(lines.front(), ' ');
    size_t total_count = 0;
    MemoTrasformer transformer;
    for (auto raw_n : raw_ns) {
        auto n = utils::parse<unsigned long long>(raw_n);
        total_count += transformer.count_after_n_transforms(n, 25);
    }
    std::cout << "Total count: " << total_count << std::endl;
    
    
    return 0;
}