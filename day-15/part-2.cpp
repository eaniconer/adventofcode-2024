#include "file_reader.hpp"
#include "world/plane/object.hpp"
#include "world/plane/direction.hpp"

#include <iostream>
#include <vector>
#include <cassert>
#include <memory>
#include <unordered_set>

namespace {

using Integer = int;
using MovableObject = world::plane::OrientedObject<Integer>;

using Robot = MovableObject;
using Box = MovableObject;
using Wall = world::plane::Object<Integer>;

using Direction = world::plane::Direction;

struct Scene {
    std::unique_ptr<Robot> robot;
    std::vector<Box> boxes;
    std::vector<Wall> walls;
};

std::pair<size_t, size_t> toRC(world::plane::Vec2<Integer> v) {
    auto [x, y] = v;
    return {static_cast<size_t>(y), static_cast<size_t>(x)};
}

void try_move_robot(Scene& scene) {
    auto& robot = *scene.robot;

    for (const auto& wall : scene.walls) {
        if (world::plane::has_collision_on_move(robot, wall))
            return;
    }

    std::vector<Box*> collided;
    for (auto& box : scene.boxes) {
        if (world::plane::has_collision_on_move(robot, box)) {
            collided.push_back(&box);
        }
    }

    if (collided.empty()) {
        robot.do_steps(1);
        return;
    }

    std::unordered_set<Box*> to_move;
    auto clear_to_move = [&to_move] {
        for (auto* box : to_move) {
            box->set_direction(Direction::No);
        }
        to_move.clear();
    };

    while (!collided.empty()) {
        Box* b = collided.back();
        collided.pop_back();

        b->set_direction(robot.direction()); 
        to_move.insert(b);

        for (const auto& wall : scene.walls) {
            if (world::plane::has_collision_on_move(*b, wall)) { // todo: fix code duplication
                clear_to_move();
                return;
            }
        }

        for (auto& other : scene.boxes) {
            if (&other == b) continue;
            if (world::plane::has_collision_on_move(*b, other)) {
                collided.push_back(&other);
            }
        }
    }

    assert(!to_move.empty());
    for (auto* box : to_move) {
        box->do_steps(1);
    }
    robot.do_steps(1);
    clear_to_move(); // reset dirs in boxes
}

}

int main() {
    static const char WALL = '#';
    static const char BOX = 'O';
    static const char ROBOT = '@';
    static const char EMPTY = '.';

    auto lines = utils::readLines("input.txt");

    const size_t width = lines.at(0).size();
    const size_t height = [&lines]() -> size_t {
        for (size_t r = 0; r < lines.size(); ++r) {
            if (lines[r].empty()) return r;
        }
        return 0;
    }();
    std::cout << "Field width=" << width << " and height=" << height << std::endl;

    auto next_command = [&lines, height]() -> Direction {
        static size_t row = height;
        static size_t col = 0;
        
        for (; row < lines.size(); ++row, col = 0) {
            for (; col < lines[row].size(); ++col) {

                const char raw_dir = lines[row][col];
                col++;
                std::cout << "Next command: " << raw_dir << std::endl;
                switch (raw_dir) {
                    case '^': return Direction::S; 
                    case 'v': return Direction::N;
                    case '>': return Direction::E;
                    case '<': return Direction::W;
                    default:
                        throw std::runtime_error("Unknown command: '" + std::string(1, lines[row][col]) + "'");
                }
            }
        }
        return Direction::No;
    };

    Scene scene;   
    for (size_t r = 0; r < height; ++r) {
        for (size_t c = 0; c < width; ++c) {
            auto x = 2 * static_cast<Integer>(c); // 2 stands to x2 width 
            auto y = static_cast<Integer>(r);
            switch (lines[r][c]) {
                case ROBOT:
                    scene.robot = std::make_unique<Robot>(Robot{{x, y}, next_command()});
                    break;
                case WALL:
                    scene.walls.push_back(Wall{{x, y}});
                    scene.walls.back().set_size({2, 1});
                    break;
                case BOX:
                    scene.boxes.push_back(Box{{x, y}, Direction::No});
                    scene.boxes.back().set_size({2, 1});
                    break;
                default:
                    assert(lines[r][c] == EMPTY);
                    //noop
            }
        }
    }
    

    std::cout << "Robot: " << *scene.robot << std::endl;

    auto print_scene = [height, width](const Scene& scene) {
        static std::vector<std::string> field(height, std::string(width * 2, EMPTY));

        static auto get_cell = [] (auto pos) -> char& {
            auto [r, c] = toRC(pos);
            return field.at(r).at(c);
        };
        
        // reset
        for (auto& line : field) {
            for (auto& cell : line) {
                if (cell == EMPTY || cell == WALL) continue;
                cell = EMPTY;
            }
        }

        // fill
        for (const auto& wall : scene.walls) {
            get_cell(wall.position()) = WALL; 
            get_cell(world::plane::shift(wall.position(), Direction::E, 1)) = WALL;
        }

        if (scene.robot) {
            {
                char& cell = get_cell(scene.robot->position());
                assert(cell == EMPTY);
                cell = ROBOT;
            }
        }
        bool catch_error = false;

        for (const auto& box : scene.boxes) {
            {
                char& cell = get_cell(box.position());
                assert(cell == EMPTY);
                cell = '[';
            }
            {
                char& cell = get_cell(world::plane::shift(box.position(), Direction::E, 1));
                assert(cell == EMPTY);
                cell = ']';
            }
        }
        


        for (const auto& line : field) { 
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    };


    print_scene(scene);
    
    while (scene.robot->direction() != Direction::No) {
        try_move_robot(scene);
        // print_scene(scene);
        scene.robot->set_direction(next_command());
    }

    size_t gps_sum = 0;

    for (const auto& box : scene.boxes) {
        auto [r, c] = toRC(box.position());
        gps_sum += (100 * r) + c;
    }

    std::cout << "GPS sum: " << gps_sum << std::endl;
        


    // auto get_cell = [&lines](auto pos) -> char& {
    //     auto [c, r] = pos;
    //     return lines.at(r).at(c);
    // };

    // std::vector<Box> boxes_to_move;
    // while (robot.direction() != Direction::No) {
    //     auto dir = world::plane::toVec2<int>(robot.direction());
    //     auto pos = robot.position();

    //     boxes_to_move.clear();
    //     pos += dir;

    //     while (true) {
    //         auto cell = get_cell(pos);
            
    //         if (cell == BOX) {
    //             boxes_to_move.emplace_back(pos);
    //             pos += dir;
    //             continue;
    //         }

    //         if (cell == EMPTY) {
    //             break;
    //         }

    //         if (cell == WALL) {
    //             boxes_to_move.clear();
    //             break;
    //         }
    //     }

    //     while (!boxes_to_move.empty()) {
    //         auto box_pos = boxes_to_move.back().position();
    //         boxes_to_move.pop_back();
    //         auto target_pos = box_pos + dir;
    //         assert(get_cell(target_pos) == EMPTY);
    //         std::swap(get_cell(target_pos), get_cell(box_pos));
    //         assert(get_cell(target_pos) == BOX);
    //     }

    //     if (get_cell(robot.position() + dir) == EMPTY)
    //         robot.do_steps(1);
       
    //     // print_field(robot);

    //     robot.set_direction(next_command());
    // }

    // size_t gps_sum = 0;

    // for (size_t r = 0; r < height; ++r) {
    //     for (size_t c = 0; c < width; ++c) {
    //         if (lines[r][c] == BOX) {
    //             gps_sum += (100 * r) + c;
    //         }
    //     }
    // }

    // std::cout << "GPS sum: " << gps_sum << std::endl;

    return 0;
}