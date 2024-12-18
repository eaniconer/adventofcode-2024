#include "file_reader.hpp"

#include <unordered_map>
#include <cassert>
#include <unordered_set>
#include <iostream>
#include <limits>

namespace {

using Integer = size_t;
using Field = decltype(utils::readLines(nullptr));

static const char WALL = '#';
static const char EMPTY = '.';
static const char START = 'S';
static const char END = 'E';

}

int main() {
    static const Integer ROTATE_COST = 1000;
    static const Integer STEP_COST = 1;

    Field field = utils::readLines("input.txt");
    const size_t h = field.size();
    const size_t w = field.at(0).size();

    auto convertRCLtoVertexIndex = [h, w](size_t r, size_t c, size_t level) {
        size_t offset = (h * w) * level;
        return (r * w + c) + offset;
    };
    auto convertVertexIndexToRC = [h, w](size_t vi) {
        vi %= (h * w);
        size_t c = vi % w;
        size_t r = vi / w;
        return std::make_pair(r, c);
    };

    std::unordered_map<size_t, std::unordered_map<size_t, size_t>> graph;
    size_t edge_count = 0;
    auto add_edge = [&graph, &edge_count](size_t u, size_t v, size_t w) {
        graph[u][v] = w;
        graph[v][u] = w;
        edge_count++;
    };

    std::pair<size_t, size_t> start;
    std::pair<size_t, size_t> end;

    bool start_found = false;
    bool end_found = false;
    for (size_t r = 0; r < h; ++r) {
        for (size_t c = 0; c < w; ++c) {
            char& value = field[r][c];
            if (value == WALL)
                continue;
            if (value == START) {
                value = EMPTY;
                start_found = true;
                start = std::make_pair(r, c);
            }
            if (value == END) {
                value = EMPTY;
                end_found = true;
                end = std::make_pair(r, c);
            }
            assert(value == EMPTY);

            auto u0 = convertRCLtoVertexIndex(r, c, 0);
            if (r > 0 && field[r - 1][c] == EMPTY) {
                auto v0 = convertRCLtoVertexIndex(r - 1, c, 0);
                add_edge(u0, v0, STEP_COST);
            }

            auto u1 = convertRCLtoVertexIndex(r, c, 1);
            if (c > 0 && field[r][c - 1] == EMPTY) {
                auto v1 = convertRCLtoVertexIndex(r, c - 1, 1);
                add_edge(u1, v1, STEP_COST);
            }

            add_edge(u0, u1, ROTATE_COST);
        }
    }
    if (!start_found)
        throw std::runtime_error("START not found on the field");
    if (!end_found)
        throw std::runtime_error("END not found on the field");

    auto [sr, sc] = start;
    auto vi = convertRCLtoVertexIndex(sr, sc, 1);

    std::cout << "Graph with |V|=" << graph.size()
        << ", |E|=" << edge_count << std::endl;

    std::unordered_map<size_t, size_t> d;
    for (const auto& [k, _] : graph)
        d[k] = std::numeric_limits<size_t>::max();
    d[vi] = 0;
    std::unordered_set<size_t> visited;

    // we have Graph with |V|=20176, |E|=20464
    // the following algorithm is very-very-very slow
    // TODO: enhance algorithm
    while (visited.size() != graph.size()) {
        if (visited.size() % 1000 == 0) {
            std::cout << "visited size = " << visited.size() << std::endl;
        }
        size_t v = std::numeric_limits<size_t>::max();
        size_t best_d = std::numeric_limits<size_t>::max();
        for (const auto& [k, _] : graph) {
            if (visited.find(k) != visited.end())
                continue;
            if (d[k] < best_d) {
                best_d = d[k];
                v = k;
            }
        }
        assert(v != std::numeric_limits<size_t>::max());
        visited.insert(v);

        const auto& us = graph[v];
        for (auto [u, w] : us) {
            if (visited.find(u) != visited.end())
                continue;
            if (d[v] + w < d[u]) {
                d[u] = d[v] + w;
            }
        }
    }

    auto [er, rc] = end;
    std::cout << " DIST: "
        << std::min(d[convertRCLtoVertexIndex(er, rc, 0)], d[convertRCLtoVertexIndex(er, rc, 1)])
        << std::endl;

    return 0;
}