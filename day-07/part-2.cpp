#include "file_reader.hpp"
#include "ostream_support.hpp"
#include "str/split.hpp"
#include "str/parse.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>

namespace {

using Integer = unsigned long long;

template <class It>
bool has_solution_impl(Integer target, It begin, It end, Integer cur_value) {
    if (begin == end) 
        return cur_value == target;

    assert(cur_value <= std::numeric_limits<Integer>::max() - *begin);
    if (has_solution_impl(target, std::next(begin), end, cur_value + *begin))   
        return true;
    
    assert(cur_value <= std::numeric_limits<Integer>::max() / *begin);
    if (has_solution_impl(target, std::next(begin), end, cur_value * *begin))
        return true;

    auto n = *begin;
    while (n) {
        cur_value *= 10;
        n /= 10;
    }
    if (has_solution_impl(target, std::next(begin), end, cur_value + *begin))
        return true;


    return false;
}

bool has_solution(Integer target_sum, const std::vector<Integer> ns) {
    if (ns.empty()) {
        return target_sum == 0;
    }
    return has_solution_impl(target_sum, std::next(ns.begin()), ns.end(), ns[0]);
}

}

int main() {
    Integer sum = 0;

    auto lines = utils::readLines("input.txt");
    for (const auto& line : lines) {
        auto parts = utils::split(line, ": ");
        assert(parts.size() == 2);
       
        Integer target_sum = utils::parse<Integer>(parts[0]);
        
        auto raw_nums = utils::split(parts[1], ' ');
        std::vector<Integer> nums;
        std::transform(
            raw_nums.begin(), 
            raw_nums.end(), 
            std::back_inserter(nums),
            utils::parse<decltype(nums)::value_type>); 
        
        if (has_solution(target_sum, nums)) {
            sum += target_sum;
        }
    }

    std::cout << "Sum: " << sum << std::endl;
}