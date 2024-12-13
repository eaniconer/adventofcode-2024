#include "file_reader.hpp"
#include "ostream_support.hpp"
#include "hash/pair_support.hpp"

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <set>

namespace {
const char EMPTY = '.';
const std::vector<std::pair<size_t, size_t>> dirs {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}
};

using Coord = size_t;
using Position = std::pair<size_t, size_t>;

class Field {
public:
    explicit Field(std::vector<std::string> data) : data_(std::move(data)) {}
    
    size_t row_count() const { return data_.size(); }
    size_t col_count() const { return data_.at(0).size(); }

    char get(Position p) const {
        auto [r, c] = p;
        if (r >= data_.size()) 
            return EMPTY;
        if (c >= data_[0].size()) 
            return EMPTY;
        return data_[r][c];
    }
    bool set(Position p, char new_value) {
        auto [r, c] = p;
        if (r >= data_.size()) 
            return false;
        if (c >= data_[0].size()) 
            return false;
        data_[r][c] = new_value;
        return true;
    }
private:
    std::vector<std::string> data_;
};

size_t process_region(Position p, Field& field) {
    assert(field.get(p) != EMPTY);
   
    auto label = field.get(p);
    std::unordered_set<Position> seen;
    std::unordered_set<Position> to_see;
    to_see.insert(p);

    // info to calc the number of sides
    std::unordered_map<std::pair<Coord, int>, std::set<Coord>> v_fences, h_fences;

    while (!to_see.empty()) {
        auto nh = to_see.extract(to_see.begin());
       
        auto [rr, cc] = nh.value();

        for (auto [dr, dc] : dirs) {
            Position np {rr + dr, cc + dc}; 
            auto dlabel = field.get(np);
            if (label == dlabel) {
                if (seen.find(np) == seen.end()) {
                    to_see.insert(np);
                }
            } else {
                auto [nr, nc] = np;
                if (dr == 0) {
                    auto fc = (nc > field.col_count()) ? 0 : std::max(cc, nc);
                    int sign = (dc == 1) ? 1 : -1;
                    v_fences[std::make_pair(fc, sign)].insert(rr);
                } else {
                    assert(dc == 0);
                    auto fr = (nr > field.row_count()) ? 0 : std::max(rr, nr);
                    int sign = (dr == 1) ? 1 : -1;
                    h_fences[std::make_pair(fr, sign)].insert(cc);
                }
            }
        }
        seen.insert(std::move(nh));
    }
    for (auto pp : seen) {
        field.set(pp, EMPTY);
    }
    
    size_t side_count = 0;

    auto count_sides = [](const auto& fences) {
        size_t count = 0;
        for (const auto& [_, vs] : fences) {
            std::optional<Coord> prev;
            for (auto it = vs.begin(); it != vs.end(); ++it) {
                if (!prev) {
                    prev = *it;
                    count++;
                    continue;
                }

                if (*prev + 1 != *it) 
                    count++;

                prev = *it;
            }
        }
        return count;
    };
    side_count += count_sides(v_fences);
    side_count += count_sides(h_fences);

    auto price = side_count * seen.size();
    std::cout << "Label " << label 
        << " price: " << price
        << "(sides=" << side_count << ", area=" << seen.size() << ")" << std::endl;
    return price;
}

}

int main() {
    Field field{utils::readLines("input.txt")};

    size_t total_price = 0;
    for (Coord r = 0; r < field.row_count(); ++r) {
        for (Coord c = 0; c < field.col_count(); ++c) {
            Position p{r, c};
            if (field.get(p) == EMPTY)
                continue;
            total_price += process_region(p, field);
        }
    }
    std::cout << "Total price: " << total_price << std::endl;

    return 0;
}