#include "file_reader.hpp"
#include "str/split.hpp"
#include "str/parse.hpp"

#include <string_view>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

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

int eval(WireName z, std::unordered_map<WireName, int>& context, const std::unordered_map<WireName, Gate>& out2gates) {
    if (context.contains(z))
        return context[z];

    auto& gate = out2gates.at(z);
    int i1 = eval(gate.in0, context, out2gates);
    int i2 = eval(gate.in1, context, out2gates);

    int r = gate.op(i1, i2);
    context[z] = r;
    return r;
}

} // namespace

int main() {
    auto lines = utils::readLines("input.txt");

    std::unordered_map<WireName, int> context;
    std::unordered_set<WireName> zs;

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
            context[ps.at(0)] = utils::parse<int>(ps.at(1));
        }
    }

    unsigned long long result = 0;
    for (auto z : zs) {
        int r = eval(z, context, out2gates);

        if (r == 1) {
            auto ps = utils::split(z, 'z');
            auto shift = utils::parse<unsigned long long>(ps.at(1));
            result |= (1ull << shift);
        }

        std::cout << z << ": " << r << " " << std::endl;

    }

    std::cout << "Result: " << result << std::endl;



    return 0;
}