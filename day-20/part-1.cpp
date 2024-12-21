#include "file_reader.hpp"

#include "world/plane/vec2.hpp"
#include "hash/pair_support.hpp"

#include <array>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <thread>
#include <chrono>

namespace {

using Integer = size_t;
using Position = world::plane::Vec2<Integer>;
using Offset = world::plane::Vec2<Integer>;
using Field = std::vector<std::string>;

std::array<Offset, 4> offsets {
    Offset{Integer(-1), 0}, Offset{1, 0}, Offset{0, 1}, Offset{0, Integer(-1)}
};

static const char START = 'S';
static const char END = 'E';
static const char WALL = '#';
static const char EMPTY = '.';

char& get_cell(Field& field, Position p) {
    auto [x, y] = p;
    return field.at(y).at(x);
}
char get_cell(const Field& field, Position p) {
    auto [x, y] = p;
    return field.at(y).at(x);
}
bool in_bounds(const Field& field, Position p) {
    auto [x, y] = p;
    return 0 <= y && y < field.size() && 0 <= x && x < field[0].size();
}

std::optional<Position> find_first(const Field& field, char cell) {
    for (size_t y = 0; y < field.size(); ++y)
        for (size_t x = 0; x < field[y].size(); ++x)
            if (get_cell(field, {x, y}) == cell)
                return Position{x, y};
    return std::nullopt;
}

[[ maybe_unused ]] void print_field(const Field& field, const std::unordered_set<Position>& to_visit) {
    Field f = field;
    for (auto p : to_visit) {
        get_cell(f, p) = 'O';
    }

    for (const auto& line : f) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;

}



std::unordered_map<Position, size_t> build_dist_index(const Field& field, Position target) {
    if (get_cell(field, target) == WALL) {
        throw std::runtime_error("Target position shouldn't be WALL");
    }

    std::unordered_map<Position, size_t>  index;

    std::unordered_set<Position> to_visit, new_to_visit;
    to_visit.insert(target);

    size_t dist = 0;
    while (!to_visit.empty()) {

        // print_field(field, to_visit);
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        new_to_visit.clear();
        for (auto p : to_visit) {
            index[p] = dist;
        }
        for (auto p : to_visit) {
            for (auto offset : offsets) {
                auto np = p + offset;
                if (index.contains(np))
                    continue;
                if (get_cell(field, np) == WALL)
                    continue;
                new_to_visit.insert(np);
            }
        }

        std::swap(new_to_visit, to_visit);
        dist++;
    }

    return index;
}

static void check_impl(bool success, const char* check_expr, const char* file, int line) {
    if (!success) {
        std::cerr << "[CHECK_FAILED]" << file << ":" << line << ": condition='" << check_expr << "'\n";
        throw std::runtime_error("CHECK FAILED");
    }
}

#define CHECK(cond) \
    do { check_impl((cond), #cond, __FILE__, __LINE__); } while(false)

Integer dist(Integer a, Integer b) {
    return (a > b) ? a - b : b - a;
}

}

int main() {
    auto field = utils::readLines("input.txt");

    Position start = find_first(field, START).value();
    Position end = find_first(field, END).value();

    get_cell(field, start) = EMPTY;
    get_cell(field, end) = EMPTY;

    const auto dist_from_start = build_dist_index(field, start);
    const auto dist_from_end = build_dist_index(field, end);

    auto start_end_dist = dist_from_end.at(start);
    CHECK(start_end_dist == dist_from_start.at(end));

    std::unordered_map<size_t, size_t> shorter_counter;

    for (auto [v, d_sv] : dist_from_start) {
        for (auto [u, d_su] : dist_from_start) {
            if (d_su <= d_sv)
                continue;
            CHECK(v != u);

            auto dx = dist(v.x, u.x);
            auto dy = dist(v.y, u.y);
            if (dx + dy != 2)
                continue;

            auto uv_steps = d_su - d_sv;
            auto shorter_count = uv_steps - 2;
            if (shorter_count > 0)
                shorter_counter[shorter_count]++;
        }
    }

    size_t best_count = 0;
    for (auto [k, v] : shorter_counter) {
        std::cout << k << ":" << v << std::endl;
        if (k >= 100)
            best_count += v;
    }

    std::cout << "Number of cheats that would save at least 100 picoseconds: " << best_count << std::endl;

    return 0;
}