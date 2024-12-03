#include <regex>
#include <fstream>
#include <iterator>
#include <iostream>
#include <cstdlib>

int main() {

    std::regex mul(R"((mul)\((\d{1,3}),(\d{1,3})\)|(do)\(\)|(don't)\(\))");

    std::ifstream inf("input.txt");
    std::string line;
    size_t mulsum = 0;
    bool is_do = true;
    while (std::getline(inf, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), mul);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            auto match = *it;

            if (match[4].str() == "do") {
                is_do = true;
            }
            if (match[5].str() == "don't") {
                is_do = false;
            }
            if (match[1].str() == "mul" && is_do) {
                int lhs = std::atoi(match[2].str().c_str());
                int rhs = std::atoi(match[3].str().c_str());
                mulsum += (lhs * rhs);
            }           
        }
    }

    std::cout << mulsum << std::endl;
    return 0;
}