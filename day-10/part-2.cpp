#include "file_reader.hpp"

#include "hash/pair_support.hpp"

#include <unordered_map>
#include <utility>
#include <iostream>

int main() {
    auto field = utils::readLines("input.txt");

    size_t scores = 0;
    for (size_t r = 0; r < field.size(); ++r) {
        for (size_t c = 0; c < field[r].size(); ++c) {
            if (field[r][c] != '0')
                continue;
            
            using CoordsMap2Count = std::unordered_map<std::pair<size_t, size_t>, size_t>;
            CoordsMap2Count old_wave, new_wave;
            old_wave[std::make_pair(r, c)] = 1;

            auto process = [&new_wave, &field] (size_t r, size_t c, char expected, size_t cnt) {
                if (r < field.size() && c < field[0].size()) {
                    if (field[r][c] == expected) {
                        new_wave[std::make_pair(r, c)] += cnt;
                    }
                }
            };
            for (char ch = '1'; ch <= '9'; ++ch) {
                for (auto [rc, cnt] : old_wave) {
                    auto [rr, cc] = rc;
                    process(rr - 1, cc, ch, cnt);
                    process(rr, cc + 1, ch, cnt);
                    process(rr + 1, cc, ch, cnt);
                    process(rr, cc - 1, ch, cnt);
                }
                old_wave.clear();
                std::swap(old_wave, new_wave);
            }
            // std::cout << old_wave.size() << " ";
            for (auto [_, cnt] : old_wave)
                scores += cnt;
        }
    }

    std::cout << "scores: " << scores << std::endl;
    
    return 0;
}