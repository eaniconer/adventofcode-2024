#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

int main() {

    ifstream in("input.txt");

    using T = long long;
    vector<int> as;
    unordered_map<int, T> bs;
    int a, b;
    while (in >> a >> b) {
        as.push_back(a);
        bs[b] += 1;
    }

    T score = 0;
    assert(as.size() == 1000);
    for (auto a : as) {
        score += a * bs[a];
    }

    std::cout << score << std::endl;

    return 0;
}