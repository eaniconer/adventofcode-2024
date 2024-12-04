
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>

namespace {

class Field {
public:
    static Field parse(std::ifstream& inf) {
        Field field;

        std::string line;
        while (std::getline(inf, line)) {
            field.rows_.push_back(std::move(line));
            assert(line.empty());
        }

        return field;
    }

    size_t nRows() const { return rows_.size(); }
    size_t nCols() const { 
        return rows_.empty() ? 0 : rows_[0].size();
    }

    char get(size_t r, size_t c) const {
        const char EMPTY = '.';

        if (r >= rows_.size()) 
            return EMPTY;
        
        if (c >= rows_[r].size()) 
            return EMPTY;

        return rows_[r][c];
    }

private:
    std::vector<std::string> rows_;
};

}

int main() {
    std::ifstream inf("input.txt");
    const auto field = Field::parse(inf);

    size_t xmas_count = 0;
    for (size_t r = 0; r < field.nRows(); ++r) {
        for (size_t c = 0; c < field.nCols(); ++c) {
            if (field.get(r, c) != 'A') 
                continue;
                
            char ne = field.get(r - 1, c + 1); // ne stands for north-east
            char se = field.get(r + 1, c + 1);
            char sw = field.get(r + 1, c - 1);
            char nw = field.get(r - 1, c - 1);

            bool main_diag_is_good =
                (nw == 'M' && se == 'S') || (nw == 'S' && se == 'M');
            bool secondary_diag_is_good =
                (ne == 'M' && sw == 'S') || (ne == 'S' && sw == 'M');

            if (main_diag_is_good && secondary_diag_is_good) {
                xmas_count++;
            }
        } 
    }

    std::cout << xmas_count << std::endl;
    
    return 0;
}