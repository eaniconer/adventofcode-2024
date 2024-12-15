#include "file_reader.hpp"
#include "plane/object.hpp"

#include <iostream>
#include <unordered_set>
#include <cassert>
#include <utility>

namespace {
using world::plane::OrientedObject;
using world::plane::Direction;
using world::plane::Vec2i;

OrientedObject<> findGuard(const std::vector<std::string>& field) {
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            if (field[i][j] == '^') {
                int y = static_cast<int>(field.size() - 1 - i);
                int x = static_cast<int>(j);
                return OrientedObject({x, y}, Direction::N);
            }
        } 
    }
    throw std::runtime_error{"Invalid input. Guard '^' not found on the field!"};
}

}
int main() {
    auto field = utils::readLines("input.txt");
    if (field.empty()) {
        throw std::runtime_error{"field should not be empty"};
    }
    auto print_field = [&] {
        for (const auto& line : field) 
            std::cout << line << std::endl;
    };

    OrientedObject guard = findGuard(field);
    const auto starting_position = guard.position();
    std::cout << guard << std::endl;

    std::unordered_set<Vec2i> visited;
    visited.emplace(guard.position());
    
    auto x_to_col = [&](int x) { return static_cast<size_t>(x); };
    auto y_to_row = [&](int y) { return field.size() - 1 - static_cast<size_t>(y); };
    auto out_of_bound = [&](auto pos) {
        return field.size() <= y_to_row(pos.y) || field[0].size() <= x_to_col(pos.x);
    };
    auto is_obstruction = [&](auto pos) {
        assert(!out_of_bound(pos));
        return field[y_to_row(pos.y)][x_to_col(pos.x)] == '#';
    };

    // simulate
    size_t step_count = 0;
    while (true) {
        guard.do_steps(1); step_count += 1;
        auto pos = guard.position();
        if (out_of_bound(pos))
            break;
        if (is_obstruction(pos)) {
            guard.do_steps(-1); step_count -= 1;
            guard.rotate90CW();
            continue;
        }
        
        visited.emplace(pos);
    }

    std::cout << "Guard did steps: " << step_count << std::endl;
    std::cout << "Unique pos count: " << visited.size() << std::endl;

    size_t cycle_count = 0;

    struct Vec2DirHash {
        size_t operator()(const std::pair<Vec2i, Direction>& p) const {
            return std::hash<Vec2i>{}(p.first) ^ std::hash<Direction>{}(p.second);
        }
    };  
    std::unordered_set<std::pair<Vec2i, Direction>, Vec2DirHash> visited2;
    for (auto p : visited) {
        if (p == starting_position) 
            continue;

        auto r = y_to_row(p.y);
        auto c = x_to_col(p.x);
        char old_value = field[r][c];
        field[r][c] = '#';

        guard.set_position(starting_position);
        guard.set_direction(Direction::N);

        visited2.clear();
        while (true) {
            guard.do_steps(1);              // todo: avoid code duplication
            auto pos = guard.position();
            if (out_of_bound(pos))
                break;
            if (is_obstruction(pos)) {
                guard.do_steps(-1); step_count -= 1;
                guard.rotate90CW();
                continue;
            }

            auto pd = std::make_pair(pos, guard.direction());
            if (visited2.find(pd) != visited2.end()) {
                cycle_count++;
                break;
            } else {
                visited2.emplace(pd);
            }
        }

        field[r][c] = old_value;
    }
    std::cout << "Cycle count: " << cycle_count << std::endl;

    return 0;
}
 