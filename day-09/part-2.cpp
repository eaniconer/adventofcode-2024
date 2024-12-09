#include "file_reader.hpp"

#include <cassert>
#include <iostream>

// #define LOG_IS_ON

#if defined(LOG_IS_ON)
#define LOG() std::cout
#else
struct FakeOut {} fake_out;

template <class T>
FakeOut& operator<<(FakeOut& out, const T&) { return out; }
#define LOG() fake_out

#endif

int main() {
    auto lines = utils::readLines("input.txt");
    
    assert(lines.size() == 1);
    std::string& line = lines.front();
    assert(!line.empty());
    std::cout << "line size: " << line.size() << std::endl;

    using Integer = unsigned long long;

    Integer checksum = 0;
    
    std::vector<size_t> lhs_count;
    lhs_count.reserve(line.size());
    size_t cum_count = 0;
    for (char ch : line) {
        lhs_count.push_back(cum_count);
        size_t d = ch - '0';
        assert(cum_count <= cum_count + d); // check overflow
        cum_count += d;
    }

    size_t back_index = line.size() - 1;
    if (back_index % 2 == 1)
        back_index--;
    
    while (back_index < line.size()) { // todo: not effective algorithm O(N^2)

        LOG() << "Considering '" << line[back_index] << "' at position " << back_index << "\n";

        if (line[back_index] == '0') {
            back_index -= 2;
            continue;
        }

        size_t n_index = back_index / 2;
        bool found = false;
        for (size_t i = 1; i < back_index; i += 2) { 
            if (line[i] >= line[back_index]) {

                LOG() << "\tFound suitable position at " << i << ", line[i]=" << line[i] << "\n";

                found = true;
                line[i] = '0' + (line[i] - line[back_index]);

                size_t d = line[back_index] - '0';
                line[back_index] = '0';

                for (size_t di = 0; di < d; ++di) {
                    checksum += lhs_count[i] * n_index;
                    LOG() << "\t\t" << lhs_count[i] << " * " << n_index << "\n";
                    lhs_count[i]++;
                }
                break;
            }
        }
        if (!found) {
            size_t d = line[back_index] - '0';
            for (size_t di = 0; di < d; ++di) {
                checksum += (lhs_count[back_index] + di) * n_index;
                LOG() << "\t\t" << lhs_count[back_index] + di << " * " << n_index << "\n";
            }

            LOG() << "\tNo move " << "\n";
        }
        back_index -= 2;
    }
    

    std::cout << "checksum: " << checksum << std::endl;

    return 0;
}