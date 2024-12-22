#include "file_reader.hpp"

#include "world/plane/vec2.hpp"
#include "unittests/assert.hpp"
#include "str/parse.hpp"
#include "hash/pair_support.hpp"

#include <unordered_map>
#include <iostream>
#include <array>

namespace {

using ULL = unsigned long long;
using Integer = int;
using Position = world::plane::Vec2<Integer>;
using Offset = world::plane::Vec2<Integer>;

using Keypad = std::vector<std::string>;
using KeypadMap = std::unordered_map<char, Position>;
KeypadMap buildKeypadMap(const std::vector<std::string>& keypad) {
    std::unordered_map<char, Position> m;

    for (Integer y = 0; y < static_cast<Integer>(keypad.size()); ++y) {
        for (Integer x = 0; x < static_cast<Integer>(keypad[y].size()); ++x) {
            m[keypad[y][x]] = Position(x, y);
        }
    }

    return m;
}

std::array<Offset, 4> offsets {
    Offset{Integer(-1), 0}, Offset{1, 0}, Offset{0, 1}, Offset{0, Integer(-1)}
};

const Keypad NUM_KEYPAD = {"789", "456", "123", " 0A"};
const Keypad DIR_KEYPAD = {" ^A", "<v>"};
const KeypadMap NUM_KEYPAD_MAP = buildKeypadMap(NUM_KEYPAD);
const KeypadMap DIR_KEYPAD_MAP = buildKeypadMap(DIR_KEYPAD);

std::vector<std::string> buildActionPathsOnKeypad(const KeypadMap& keypad_map, char from, char to) {
    const auto f_pos = keypad_map.at(from);
    const auto t_pos = keypad_map.at(to);

    Position min_pos(std::min(f_pos.x, t_pos.x), std::min(f_pos.y, t_pos.y));
    Position max_pos(std::max(f_pos.x, t_pos.x), std::max(f_pos.y, t_pos.y));
    auto in_bound = [&](Position p) -> bool {
        return min_pos.x <= p.x &&
            p.x <= max_pos.x &&
            min_pos.y <= p.y &&
            p.y <= max_pos.y;
    };

    std::unordered_map<Position, std::vector<std::string>> paths, tmp_paths;
    paths[f_pos] = {""};

    while (!paths.contains(t_pos)) {
        tmp_paths.clear();
        for (const auto& [p, p_paths] : paths) {
            for (auto offset : offsets) {
                auto np = p + offset;
                if (!in_bound(np))
                    continue;
                if (keypad_map.at(' ') == np)
                    continue;
                char c = '#'; // bad char
                if (offset.x > 0) c = '>';
                if (offset.x < 0) c = '<';
                if (offset.y > 0) c = 'v';
                if (offset.y < 0) c = '^';

                auto& np_paths = tmp_paths[np];
                for (auto p_path : p_paths) {
                    np_paths.emplace_back(std::move(p_path));
                    np_paths.back().push_back(c);
                }
            }
        }
        std::swap(paths, tmp_paths);
    }

    return std::move(paths[t_pos]);
}

std::unordered_map<std::pair<size_t, std::string>, ULL> cache;

ULL findShortestPath(std::vector<const KeypadMap*> kms, std::string keypad_chars) {
    if (kms.empty()) {
        return keypad_chars.size();
    }
    auto it = cache.find(std::make_pair(kms.size(), keypad_chars));
    if (it != cache.end())
        return it->second;

    char prev = 'A';
    const KeypadMap& keypad_map = *kms.back();
    kms.pop_back();

    ULL shortest = 0;
    for (char c : keypad_chars) {
        auto paths = buildActionPathsOnKeypad(keypad_map, prev, c);
        for (auto& p : paths)
            p.push_back('A');

        ULL shortest_subpath_size = -1;
        for (const auto& p : paths) {
            auto r = findShortestPath(kms, p);
            if (r < shortest_subpath_size)
                shortest_subpath_size = r;
        }
        shortest += shortest_subpath_size;
        prev = c;
    }

    cache[std::make_pair(kms.size() + 1, keypad_chars)] = shortest;
    return shortest;
}

namespace unittests
{

void testBuildActionPathsOnKeypad() {
    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, 'A', 'A');
        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == "");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, 'A', '0');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == "<");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, '0', 'A');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == ">");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, '1', '0');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == ">v");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, '0', '1');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == "^<");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, '5', '0');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == "vv");
    }

    {
        auto paths = buildActionPathsOnKeypad(NUM_KEYPAD_MAP, '3', '0');

        ASSERT(paths.size() == 2);
    }

    {
        auto paths = buildActionPathsOnKeypad(DIR_KEYPAD_MAP, '^', '<');

        ASSERT(paths.size() == 1);
        ASSERT(paths[0] == "v<");
    }

    {
        auto paths = buildActionPathsOnKeypad(DIR_KEYPAD_MAP, '^', '>');

        ASSERT(paths.size() == 2);
    }

    {
        auto paths = buildActionPathsOnKeypad(DIR_KEYPAD_MAP, '<', 'A');

        ASSERT(paths.size() == 2);
    }
}

void run() {
    testBuildActionPathsOnKeypad();
}

} // namespace unittests

}

int main() {
    unittests::run();

    const auto codes = utils::readLines("input.txt");

    ULL total_complexity = 0;

    std::vector<const KeypadMap*> maps;
    for (size_t i = 0; i < 25; ++i)
        maps.push_back(&DIR_KEYPAD_MAP);
    maps.push_back(&NUM_KEYPAD_MAP);

    for (const auto& code : codes) {
        cache.clear();
        const auto shortest_path_size = findShortestPath(maps, code);
        auto complexity = utils::parse<ULL>(code) * shortest_path_size;
        std::cout << code << " " << utils::parse<ULL>(code) << " " << shortest_path_size << std::endl;
        total_complexity += complexity;
    }

    std::cout << "Total complexity: " << total_complexity << std::endl;

    return 0;
}