#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"

#include "world/plane/vec2.hpp"

#include <cassert>
#include <ostream>
#include <iostream>
#include <sstream>

namespace {

using Integer = long long;
using Prize = world::plane::Vec2<Integer>;
using Button = world::plane::Vec2<Integer>;
using Claw = world::plane::Vec2<Integer>;

auto parse_object(std::string_view sv, char delim) {
    auto parts = utils::split(sv, ", ");
    assert(parts.size() == 2);
    auto x = utils::parse<Integer>(utils::split(parts[0], delim).at(1));
    auto y = utils::parse<Integer>(utils::split(parts[1], delim).at(1));
    return world::plane::Vec2<Integer>{x, y};
}

Prize parse_prize(std::string_view sv) {
    return parse_object(sv, '=');
}

Button parse_button(std::string_view sv) {
    return parse_object(sv, '+');
}

size_t solve(Button a, Button b, Prize p) {
    
    const size_t BUTTON_A_COST = 3; // tokens
    const size_t BUTTON_B_COST = 1; // tokens;
    
    auto R = (p.y * a.x - p.x * a.y);
    auto T = (b.y * a.x - b.x * a.y);

    if (R % T == 0) {
        auto b_count = R / T;
        auto V = p.x - (b_count * b.x);
        if (V % a.x == 0) {
            auto a_count = V / a.x;
            return a_count * 3 + b_count;
        }
    }
    return 0;
}

}

int main() {
    auto lines = utils::readLines("input.txt");
    
    Button button_a, button_b;
    Prize prize;

    size_t tokens = 0;
    for (const auto& line : lines) {
        if (line.empty()) continue;

        auto parts = utils::split(line, ": ");
        assert(parts.size() == 2);

        if (parts[0] == "Button A") {           // todo: enhance parsing code
            button_a = parse_button(parts[1]);
        } else if (parts[0] == "Button B") {
            button_b = parse_button(parts[1]);
        } else if (parts[0] == "Prize") {
            prize = parse_prize(parts[1]);

            prize.x += 10000000000000ll;
            prize.y += 10000000000000ll;

            tokens += solve(button_a, button_b, prize);
        } else {
            throw std::runtime_error("Unexpected input: " + line);
        }
    }

    std::cout << "Tokens spend: " << tokens << std::endl;

    return 0;
}