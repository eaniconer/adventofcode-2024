#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

int main() {

    ifstream in("input.txt");

    vector<int> as, bs;
    int a, b;
    while (in >> a >> b) {
        as.push_back(a);
        bs.push_back(b);
    }

    sort(as.begin(), as.end());
    sort(bs.begin(), bs.end());

    assert(as.size() == bs.size());

    long long cum_dist = 0;
    for (size_t i = 0; i < as.size(); ++i) {
        int dist = (as[i] - bs[i]);
        if (dist < 0)
            dist = -1 * dist;
        cum_dist += dist;
    }

    cout << cum_dist << endl;

    return 0;
}