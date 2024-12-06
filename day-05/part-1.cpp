
#include <fstream>
#include <cassert>
#include <vector>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace {

std::vector<std::string_view> split(std::string_view s, char sep) {
    std::vector<std::string_view> pages;
    while (true) {
        auto pos = s.find(sep);
        pages.push_back(s.substr(0, pos));
        if (pos == s.npos)
            break;
        s.remove_prefix(pos + 1);
    }

    return pages;
}

void test_split() {
    {
        auto pages = split("word", ',');
        assert(pages.size() == 1);
        assert(pages[0] == "word");
    }
    {
        auto pages = split("a,b", ',');
        assert(pages.size() == 2);
        assert(pages[0] == "a");
        assert(pages[1] == "b");
    }
}

}

int main() {
    test_split();

    std::vector<std::string> ordering_rules;

    std::ifstream inf("input.txt");
    std::string line;
    while (std::getline(inf, line)) {
        if (line.empty()) {
            // Ordering rules have been read!
            break;
        }
        ordering_rules.push_back(std::move(line));
    }

    std::unordered_multimap<std::string_view, std::string_view> order_index;
    for (const auto& rule : ordering_rules) {
        auto pages = split(rule, '|');
        assert(pages.size() == 2);
        order_index.emplace(pages[1], pages[0]);
    }

    // checking updates
    size_t res = 0;
    while (std::getline(inf, line)) {
        // std::cout << line;

        std::unordered_set<std::string_view> forbidden;
        bool valid = true;

        auto pages = split(line, ',');
        for (auto page : pages) {
            if (forbidden.find(page) != forbidden.end()) {
                // std::cout << " not valid because page " << page << " found in forbidden\n";
                valid = false;
                break;
            }
            auto [beg, end] = order_index.equal_range(page);
            for (auto it = beg; it != end; ++it) {
                forbidden.emplace(it->second);
            }
        }
        if (valid) {
            // std::cout << "valid\n";
            auto mid = pages.size() / 2;
            assert(mid < pages.size());
            assert(pages.size() % 2 == 1);

            res += atoi(std::string(pages[mid]).c_str());
            
        }
    }

    std::cout << res << std::endl;

    return 0;
}