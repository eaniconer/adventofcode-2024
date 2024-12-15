#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"
#include "world/plane/vec2.hpp"

#include <cassert>
#include <iostream>

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
    const Integer WIDTH = 101;
    const Integer HEIGHT = 103;
    const Integer N_STEPS = 100;

    auto lines = utils::readLines("input.txt");

    size_t ul = 0, ur = 0, dl = 0, dr = 0;

    // std::vector<std::string> field;
    // for (size_t r = 0; r < HEIGHT; ++r) {
    //     field.push_back(std::string(WIDTH, '.'));
    // }

    for (const auto& line : lines) {
        auto parts = utils::split(line, ' ');
        assert(parts.size() == 2);

        auto p = parse_vec2(parts[0]);
        auto v = parse_vec2(parts[1]);

        // std::cout << p << " " << v << std::endl;

        auto tp = p + (N_STEPS * v);
        // std::cout << tp << std::endl;

        auto x = tp.x % WIDTH;
        if (x < 0) x += WIDTH;
        auto y = tp.y % HEIGHT;
        if (y < 0) y += HEIGHT;
        // std::cout << x << " " << y << std::endl;

        assert(WIDTH % 2 == 1);
        assert(HEIGHT % 2 == 1);

        if (x < WIDTH / 2) {
            if (y < HEIGHT / 2) ul++;
            else if (y > HEIGHT / 2) dl++;
        } else if (x > WIDTH / 2) { 
            if (y < HEIGHT / 2) ur++;
            else if (y > HEIGHT / 2) dr++;
        }

        // if (field[y][x] == '.') {
        //     field[y][x] = '1';
        // } else {
        //     field[y][x]++;
        // }
    }

    // std::cout << ul << " " << ur << " " << dl << " " << dr << std::endl;
    std::cout << "Safety factor: " << (ul * ur * dl * dr) << std::endl;
    
    // for (const auto& l : field) {
    //     std::cout << l << std::endl;
    // }
    // std::cout << std::endl;

    return 0;
}