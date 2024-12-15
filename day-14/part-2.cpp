#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"
#include "world/plane/vec2.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace {

using Integer = long long;
using Vec2 = world::plane::Vec2<Integer>;

Vec2 parse_vec2(std::string_view sv) {
    auto parts = utils::split(sv, "=");
    assert(parts.size() == 2);

    auto raw_coords = utils::split(parts[1], ',');
    assert(raw_coords.size() == 2);
    Integer x = utils::parse<Integer>(raw_coords[0]);
    Integer y = utils::parse<Integer>(raw_coords[1]);

    return Vec2{x, y};
}

}

int main() {

    // static const Integer WIDTH = 11;
    // static const Integer HEIGHT = 7;

    static const Integer WIDTH = 101;
    static const Integer HEIGHT = 103;

    assert(WIDTH % 2 == 1);
    assert(HEIGHT % 2 == 1);

    auto lines = utils::readLines("input.txt");

    std::vector<Vec2> ps; //positions
    std::vector<Vec2> vs; //velocities

    ps.reserve(lines.size());
    vs.reserve(lines.size());

    for (const auto& line : lines) {
            auto parts = utils::split(line, ' ');
            assert(parts.size() == 2);
            ps.push_back(parse_vec2(parts[0]));
            vs.push_back(parse_vec2(parts[1]));
    }

    static const char EMPTY = ' ';
    std::vector<std::string> field;
    for (size_t r = 0; r < HEIGHT; ++r) {
        field.push_back(std::string(WIDTH, EMPTY));
    }
    
    auto reset_field = [&field] {
        for (auto& row : field) 
            std::fill(row.begin(), row.end(), EMPTY);
    };
    auto print_field = [&field] {
        for (const auto& row : field) {
            std::cout << row << std::endl;
        }
        std::cout << std::endl << std::endl;
    };

    auto likely_christmas_tree = [&field] {
       
        for (size_t r = 0; r < HEIGHT / 2; ++r) {
            size_t count = 0;
            for (auto cell : field[r]) {
                if (cell != EMPTY) {
                    count++;
                }
            }
            if (count > 30) return true;

        }

        return false;
    };

    for (size_t i = 1; i <= 500'000'000; ++i) {
        if (i % 1'000'000 == 0) {
            std::cout << (i / 1'000'000) << " million(s) iter(s) passed" << std::endl;
        }

        reset_field();
        for (size_t j = 0; j < ps.size(); ++j) {
            auto p = ps[j];
            auto v = vs[j];

            auto tp = p + v;
            ps[j] = tp;
           
            tp.x %= WIDTH;
            if (tp.x < 0) tp.x += WIDTH;

            tp.y %= HEIGHT;
            if (tp.y < 0) tp.y += HEIGHT;

            auto& cell = field[tp.y][tp.x];
            if (cell == EMPTY) {
                cell = '1';
            } else {
                cell++;
            }
            
        }

        if (likely_christmas_tree()) {
            std::cout << "Likely christmas tree on Iter #" << i << std::endl;
            print_field();

           // std::this_thread::sleep_for(std::chrono::milliseconds(500));
           return 0;
        }
        
    }

    return 0;
}

