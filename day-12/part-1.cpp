#include "file_reader.hpp"
#include "ostream_support.hpp"
#include "hash/pair_support.hpp"

#include <iostream>
#include <cassert>

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
    size_t perimeter = 0;

    auto label = field.get(p);
    std::unordered_set<Position> seen;
    std::unordered_set<Position> to_see;
    to_see.insert(p);

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
                perimeter++;
            }
        }
        seen.insert(std::move(nh));
    }
    for (auto pp : seen) {
        field.set(pp, EMPTY);
    }

    auto price = perimeter * seen.size();
    std::cout << "Label " << label 
        << " price: " << price
        << "(p=" << perimeter << ", area=" << seen.size() << ")" << std::endl;
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