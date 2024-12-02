#include <vector>
#include <fstream>
#include <optional>
#include <sstream>
#include <iostream>

namespace {
using Report = std::vector<int>;

std::optional<Report> tryParseReport(std::ifstream& inf) {
    std::string line;
    if (std::getline(inf, line)) {
        std::istringstream iss(line);
        std::vector<int> ns;
        int n;
        while (iss >> n) {
            ns.push_back(n);
        }
        return ns;
    }
    return std::nullopt;
}

bool reportIsSafe(const Report& report) {
    if (report.size() < 2) 
        return true;

    int first_diff = 0;
    for (size_t i = 1; i < report.size(); ++i) {
        int diff = report[i] - report[i - 1];
        if (diff == 0) 
            return false;

        if (first_diff == 0)
            first_diff = diff;

        if (diff < -3  || 3 < diff) {
            return false;
        }   

        if (first_diff * diff < 0) { 
            return false;
        }     
    }

    return true;
}

}

int main() {

    std::ifstream inf("input.txt");
    unsigned cnt = 0;
    while (auto report = tryParseReport(inf)) {
        if (reportIsSafe(*report)) {
            cnt++;
        }
    }
    std::cout << cnt << std::endl;

    return 0;
}