#include <vector>
#include <fstream>
#include <optional>
#include <sstream>
#include <iostream>
#include <algorithm>

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

bool reportIsSafe(const Report& rs) {
    if (rs.size() < 2) 
        return true;

    // O(n^2)
    std::vector<int> ds;
    ds.reserve(rs.size() - 1);

    for (size_t p = 0; p < rs.size(); ++p) {
        const int* prev = nullptr;
        ds.clear();
        
        for (size_t i = 0; i < rs.size(); ++i) {
            if (i == p) continue;
            if (!prev) {
                prev = &rs[i];
                continue;
            }
            ds.push_back(rs[i] - *prev);
            prev = &rs[i];
        }

        bool ds_is_good = true;
        size_t n_pos = 0, n_neg = 0;
        for (auto d : ds) {
            if (d < -3 || 3 < d) {
                ds_is_good = false;
                break;
            }
            n_pos += d > 0 ? 1 : 0;
            n_neg += d < 0 ? 1 : 0;
        }
        if (ds_is_good) {
            if (n_pos == ds.size() || n_neg == ds.size()) {
                return true;
            }
        }

    }
    
    return false;
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