#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"

#include "world/plane/vec2.hpp"

#include <cassert>
#include <ostream>
#include <iostream>
#include <sstream>

namespace {

using Prize = world::plane::Vec2i;
using Button = world::plane::Vec2i;
using Claw = world::plane::Vec2i;

auto parse_object(std::string_view sv, char delim) {
    auto parts = utils::split(sv, ", ");
    assert(parts.size() == 2);
    int x = utils::parse<int>(utils::split(parts[0], delim).at(1));
    int y = utils::parse<int>(utils::split(parts[1], delim).at(1));
    return world::plane::Vec2i{x, y};
}

Prize parse_prize(std::string_view sv) {
    return parse_object(sv, '=');
}

Button parse_button(std::string_view sv) {
    return parse_object(sv, '+');
}

size_t solve(Button btn_a, Button btn_b, Prize p) {

    const size_t BUTTON_A_COST = 3; // tokens
    const size_t BUTTON_B_COST = 1; // tokens;
    const size_t MAX_BUTTON_PRESS_COUNT = 100; // times

    if (MAX_BUTTON_PRESS_COUNT * btn_a.x + MAX_BUTTON_PRESS_COUNT * btn_b.x < p.x) {
        return 0;
        // throw std::runtime_error("Unreachable prize on axis X");
    }
    if (MAX_BUTTON_PRESS_COUNT * btn_a.y + MAX_BUTTON_PRESS_COUNT * btn_b.y < p.y) {
        return 0;
        // throw std::runtime_error("Unreachable prize on axis Y");
    }

    size_t best_price = -1;
    for (size_t i = 0; i <= MAX_BUTTON_PRESS_COUNT; ++i) {
        size_t price = i * BUTTON_A_COST;
        auto t = p - (btn_a * i);

        if (t.x < 0 || t.y < 0) 
            continue;

        auto x_count = t.x / btn_b.x;
        auto y_count = t.y / btn_b.y;

        if (x_count != y_count) continue;
        if (t.x % btn_b.x != 0) continue;
        if (t.y % btn_b.y != 0) continue;

        price += x_count * BUTTON_B_COST;

        if (price < best_price) 
            best_price = price;
    }

    if (best_price == -1)
        return 0;
    return best_price;
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
            tokens += solve(button_a, button_b, prize);
        } else {
            throw std::runtime_error("Unexpected input: " + line);
        }
    }

    std::cout << "Tokens spend: " << tokens << std::endl;

    return 0;
}