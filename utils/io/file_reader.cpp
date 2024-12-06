#include "file_reader.hpp"

#include <fstream>

namespace utils {

std::vector<std::string> readLines(const char* filename) {
    std::vector<std::string> lines;
    std::ifstream inf(filename);
    std::string line;
    while (std::getline(inf, line)) {
        lines.push_back(std::move(line));
    }
    return lines;
}

}