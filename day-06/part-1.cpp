#include "file_reader.hpp"
#include "plane/object.hpp"

#include <iostream>
#include <unordered_set>
#include <cassert>

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
                return OrientedObject<>({x, y}, Direction::N);
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

    OrientedObject guard = findGuard(field);
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

    return 0;
}
 