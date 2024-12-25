#include "file_reader.hpp"
#include "str/split.hpp"
#include "str/parse.hpp"
#include "ostream_support.hpp"

#include <string_view>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

namespace {

using WireName = std::string_view;

using GateOp = int(*)(int, int);

int andOp(int a, int b) { return a & b; }
int orOp(int a, int b) { return a | b; }
int xorOp(int a, int b) { return a ^ b; }

struct Gate {
    WireName in0;
    WireName in1;
    WireName out;
    GateOp op;
};

GateOp parseGateOp(std::string_view sv) {
    if (sv == "XOR") return xorOp;
    if (sv == "AND") return andOp;
    if (sv == "OR") return orOp;

    throw std::runtime_error{"Unexpected gate op"};
}

Gate parseGate(std::string_view sv) {
    // example: ntg XOR fgs -> mjb
    auto ps = utils::split(sv, ' ');

    return Gate{.in0 = ps[0], .in1 = ps[2], .out = ps[4], .op = parseGateOp(ps[1])};
}

std::string opToString(GateOp op) {
    if (op == xorOp) return "XOR";
    if (op == orOp) return "OR";
    if (op == andOp) return "AND";
    return "UNKNOWN";
}

std::string toString(WireName z, const std::unordered_map<WireName, Gate>& out2gates, int depth) {
    if (depth == 0) return std::string(z);

    if (out2gates.contains(z)) {
        const auto& gate = out2gates.at(z);
        auto lhs = toString(gate.in0, out2gates, depth - 1);
        auto rhs = toString(gate.in1, out2gates, depth - 1);

        for (char c : rhs) {
            if (isdigit(c)) {
                std::swap(rhs, lhs);
                break;
            }
        }
        if (lhs.size() > rhs.size())
            std::swap(lhs, rhs);

        return "(" + lhs + " " + opToString(gate.op) + " " + rhs + ")[" + std::string(gate.out) + "]";
    } else {
        return std::string(z);
    }

}

} // namespace

int main() {
    auto lines = utils::readLines("input.txt");

    // std::unordered_map<WireName, int> context;
    std::set<WireName> zs;

    std::unordered_map<WireName, Gate> out2gates;

    bool read_gates = false;
    for (const auto& line : lines) {
        if (line.empty()) {
            read_gates = true;
            continue;
        }
        if (read_gates) {
            auto gate = parseGate(line);
            auto out = gate.out;

            out2gates[out] = std::move(gate);

            if (out.starts_with("z")) {
                zs.insert(out);
            }

        } else {
            auto ps = utils::split(line, ": ");
            // context[ps.at(0)] = utils::parse<int>(ps.at(1));
        }
    }

    for (auto z : zs) {
        if (z < "z02") continue; // checked manually (z00, z01)

        const auto& gate = out2gates[z];
        // should be xor
        if (gate.op != xorOp) {
            if (z.ends_with("45")) continue;

            std::cout << "ERROR: " << z << " is not out of XOR, but it should be. Actual op: ";
            if (gate.op == andOp)
                std::cout << "AND";
            if (gate.op == orOp)
                std::cout << "OR";
            std::cout << std::endl;
            continue;

            /*
            on my input:
                ERROR: z14 is not out of XOR, but it should be. Actual op: OR
                ERROR: z31 is not out of XOR, but it should be. Actual op: AND
                ERROR: z35 is not out of XOR, but it should be. Actual op: AND
                ERROR: z45 is not out of XOR, but it should be. Actual op: OR
            */
        }

    }

    for (const auto& [o, gate] : out2gates) {
        if (gate.op == xorOp) {
            if (gate.in0.starts_with("x") || gate.in0.starts_with("y")) {
                auto num = gate.in0.substr(1);

                if (gate.in1.ends_with(num)) {
                    continue;
                }

            } else {
                if (gate.out.starts_with("z")) continue;
            }

            std::cout << "Strange XOR: " << gate.in0 << " XOR " << gate.in1 << " = " << gate.out << std::endl;
        }
    }
    /* got 3 pairs now
ERROR: z14 is not out of XOR, but it should be. Actual op: OR
ERROR: z31 is not out of XOR, but it should be. Actual op: AND
ERROR: z35 is not out of XOR, but it should be. Actual op: AND
Strange XOR: bbc XOR jkb = sgj
Strange XOR: nhg XOR ttd = vss
Strange XOR: nrr XOR sms = kpp
    */

    for (auto z : zs) {
        // if (z == "z45") continue;
        // if (z < "z02") continue;

        std::cout << z << ": " << toString(z, out2gates, 3) << std::endl;
    }
    // now we can manually find swapped pairs
    // 1. z35 & sgj // fix input and analyse output
    // 2. vss & z14 // fix input and analyse output
    // 3. z31 & kpp // fix input and analyse output
    // 4. kdh & hjf //

    std::vector<std::string_view> wns { "z35", "sgj", "vss", "z14", "z31", "kpp", "kdh", "hjf"};

    std::sort(wns.begin(), wns.end());
    std::cout << wns << std::endl;

    return 0;
}