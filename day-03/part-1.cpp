#include <regex>
#include <fstream>
#include <iterator>
#include <iostream>
#include <cstdlib>

int main() {

    std::regex mul(R"(mul\((\d{1,3}),(\d{1,3})\))");

    std::ifstream inf("input.txt");
    std::string line;
    size_t mulsum = 0;
    while (std::getline(inf, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), mul);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            auto match = *it;
            int lhs = std::atoi(match[1].str().c_str());
            int rhs = std::atoi(match[2].str().c_str());
            mulsum += (lhs * rhs);
        }
    }

    std::cout << mulsum << std::endl;
    return 0;
}