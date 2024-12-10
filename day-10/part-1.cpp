#include "file_reader.hpp"

#include "hash/pair_support.hpp"

#include <unordered_set>
#include <utility>
#include <iostream>

int main() {
    auto field = utils::readLines("input.txt");

    size_t scores = 0;
    for (size_t r = 0; r < field.size(); ++r) {
        for (size_t c = 0; c < field[r].size(); ++c) {
            if (field[r][c] != '0')
                continue;
            
            using CoordsSet = std::unordered_set<std::pair<size_t, size_t>>;
            CoordsSet old_wave, new_wave;
            old_wave.emplace(std::make_pair(r, c));

            auto process = [&new_wave, &field] (size_t r, size_t c, char expected) {
                if (r < field.size() && c < field[0].size()) {
                    if (field[r][c] == expected) {
                        new_wave.emplace(r, c);
                    }
                }
            };
            for (char ch = '1'; ch <= '9'; ++ch) {
                for (auto [rr, cc] : old_wave) {
                    process(rr - 1, cc, ch);
                    process(rr, cc + 1, ch);
                    process(rr + 1, cc, ch);
                    process(rr, cc - 1, ch);
                }
                old_wave.clear();
                std::swap(old_wave, new_wave);
            }
            // std::cout << old_wave.size() << " ";
            scores += old_wave.size();
        }
    }

    std::cout << "scores: " << scores << std::endl;
    
    return 0;
}