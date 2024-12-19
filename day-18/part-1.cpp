#include "file_reader.hpp"

#include "str/split.hpp"
#include "str/parse.hpp"
#include "world/plane/vec2.hpp"

#include <unordered_set>
#include <iostream>
#include <array>
#include <vector>

#include <thread>
#include <chrono>

namespace {

using Integer = size_t;
using ByteAddr = world::plane::Vec2<Integer>;

static const size_t CORRUPTED_BYTE_COUNT = 1024;
static const Integer MAX_X = 70, MAX_Y = 70;

// static const size_t CORRUPTED_BYTE_COUNT = 12;
// static const Integer MAX_X = 6, MAX_Y = 6;

void printField(const std::unordered_set<ByteAddr> corrupted, const std::unordered_set<ByteAddr>& wave) {
    std::vector<std::string> field(MAX_Y + 1, std::string(MAX_X + 1, '.'));

    for (auto [x, y] : corrupted) {
        field[y][x] = '#';
    }
    for (auto [x, y] : wave) {
        if (field[y][x] != '.')
            throw std::runtime_error("Cell is not empty");
        field[y][x] = 'O';
    }

    for (const auto& line : field)
        std::cout << line << std::endl;
    std::cout << std::endl;
}

}

int main() {


    auto lines = utils::readLines("input.txt");

    const auto corrupted_bytes = [&lines] {
        std::unordered_set<ByteAddr> bytes;
        for (size_t i = 0; i < CORRUPTED_BYTE_COUNT; ++i) {
            auto ps = utils::split(lines[i], ',');
            auto x = utils::parse<Integer>(ps.at(0));
            auto y = utils::parse<Integer>(ps.at(1));
            bytes.emplace(ByteAddr{x, y});
        }
        return bytes;
    }();

    ByteAddr start{0, 0};
    ByteAddr end{MAX_X, MAX_Y};


    size_t step_count = 1; // take into account start-pos
    std::array<ByteAddr, 4> offsets {
        ByteAddr{Integer(-1), 0}, ByteAddr{1, 0}, ByteAddr{0, 1}, ByteAddr{0, Integer(-1)}
    };

    auto is_good_byte_addr = [&corrupted_bytes](ByteAddr ba) {
        return
            0 <= ba.x && ba.x <= MAX_X &&
            0 <= ba.y && ba.y <= MAX_Y &&
            !corrupted_bytes.contains(ba);
    };

    std::unordered_set<ByteAddr> visited;
    std::unordered_set<ByteAddr> to_visit;
    to_visit.insert(start);

    while (true) {
        printField(corrupted_bytes, to_visit);

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::unordered_set<ByteAddr> new_wave;

        bool end_found = false;

        for (auto ba : to_visit) {
            visited.insert(ba);

            for (auto offset : offsets) {
                auto neib = ba + offset;
                if (!is_good_byte_addr(neib))
                    continue;

                if (neib == end) {
                    end_found = true;
                }
                if (visited.contains(neib))
                    continue;
                new_wave.insert(neib);
            }
        }

        if (end_found) {
            break;
        }
        step_count++;
        to_visit = std::move(new_wave);
    }

    std::cout << "#steps: " << step_count << std::endl;

    return 0;
}