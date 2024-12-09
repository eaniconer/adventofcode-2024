#include "file_reader.hpp"

#include <cassert>
#include <iostream>

int main() {
    auto lines = utils::readLines("input.txt");
    
    assert(lines.size() == 1);
    std::string& line = lines.front();
    assert(!line.empty());
    std::cout << "line size: " << line.size() << std::endl;

    using Integer = unsigned long long;

    Integer checksum = 0;
    size_t cur_checksum_index = 0;

    size_t back_index = line.size() - 1;
    if (back_index % 2 == 1)
        back_index--;
    
    for (size_t i = 0; i < line.size(); ++i) {
        if (i % 2 == 0) {
            size_t n_index = i / 2;
            for (char c = '0'; c < line[i]; ++c) {
                checksum += (cur_checksum_index * n_index);
                cur_checksum_index++;
            }
        } else {
            while (line[i] != '0') {
                if (back_index < i) break;

                assert(back_index > 1);
                if (line[back_index] == '0') {
                    back_index -= 2;
                    continue;
                }

                assert(line[back_index] != '0');
                size_t n_index = back_index / 2;
                checksum += (cur_checksum_index * n_index);
                cur_checksum_index++;

                line[back_index]--;
                line[i]--;
            }
        }
    }

    std::cout << "checksum: " << checksum << std::endl;

    return 0;
}