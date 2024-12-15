#include "file_reader.hpp"
#include "world/plane/object.hpp"
#include "world/plane/direction.hpp"

#include <iostream>
#include <vector>
#include <cassert>

namespace {

using MovableObject = world::plane::OrientedObject<>;
using Robot = MovableObject;
using Box = world::plane::Object<>;
using Direction = world::plane::Direction;

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

    auto print_field = [&lines, height](const Robot& robot) {
        auto [rc, rr] = robot.position();
        for (size_t r = 0; r < height; ++r) {
            if (static_cast<size_t>(rr) == r) {
                for (size_t c = 0; c < lines[r].size(); ++c) {
                    if (static_cast<size_t>(rc) == c) std::cout << ROBOT;
                    else std::cout << lines[r][c];
                }
                std::cout << std::endl;
            } else {
                std::cout << lines[r] << std::endl;
            }
        }
        std::cout << std::endl;
    };


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

    Robot robot = [&]() {
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                if (lines[r][c] == ROBOT) {
                    lines[r][c] = EMPTY;
                    return Robot{{c, r}, next_command()};
                }
            }
        }
        throw std::runtime_error("robot not found on the field");
    }();

    std::cout << "Robot: " << robot << std::endl;
    print_field(robot);

    auto get_cell = [&lines](auto pos) -> char& {
        auto [c, r] = pos;
        return lines.at(r).at(c);
    };

    std::vector<Box> boxes_to_move;
    while (robot.direction() != Direction::No) {
        auto dir = world::plane::toVec2<int>(robot.direction());
        auto pos = robot.position();

        boxes_to_move.clear();
        pos += dir;

        while (true) {
            auto cell = get_cell(pos);
            
            if (cell == BOX) {
                boxes_to_move.emplace_back(pos);
                pos += dir;
                continue;
            }

            if (cell == EMPTY) {
                break;
            }

            if (cell == WALL) {
                boxes_to_move.clear();
                break;
            }
        }

        while (!boxes_to_move.empty()) {
            auto box_pos = boxes_to_move.back().position();
            boxes_to_move.pop_back();
            auto target_pos = box_pos + dir;
            assert(get_cell(target_pos) == EMPTY);
            std::swap(get_cell(target_pos), get_cell(box_pos));
            assert(get_cell(target_pos) == BOX);
        }

        if (get_cell(robot.position() + dir) == EMPTY)
            robot.do_steps(1);
       
        // print_field(robot);

        robot.set_direction(next_command());
    }

    size_t gps_sum = 0;

    for (size_t r = 0; r < height; ++r) {
        for (size_t c = 0; c < width; ++c) {
            if (lines[r][c] == BOX) {
                gps_sum += (100 * r) + c;
            }
        }
    }

    std::cout << "GPS sum: " << gps_sum << std::endl;

    return 0;
}