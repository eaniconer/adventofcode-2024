#include "file_reader.hpp"
#include "str/split.hpp"
#include "str/parse.hpp"
#include "io/ostream_support.hpp"

#include <sstream>
#include <iostream>
#include <array>
#include <string_view>
#include <cassert>
#include <algorithm>

namespace {

using Integer = long long;
using Program = std::vector<Integer>;

struct Context {
    Integer a = 0;
    Integer b = 0;
    Integer c = 0;

    Integer ip = 0;
};

Integer parseRegister(std::string_view sv, char excected_name) {
    auto parts = utils::split(sv, ": ");
    if (parts.at(0).back() != excected_name) {
        std::stringstream ss;
        ss << "Unexpected register name! Excpected '" << excected_name << "', but given " << parts[0];
        throw std::runtime_error(ss.str());
    }
    return utils::parse<Integer>(parts.at(1));
}

namespace ops {

using CallType = void(Context&, Integer);
static std::array<CallType*, 8> opcode2call;

int registerCall(int opcode, CallType* call) {
    opcode2call.at(opcode) = call;
    return opcode;
}
#define REGISTER_CALL(name, opcode) \
    static const int name ## _opcode = registerCall(opcode, &name);

/*
The value of a combo operand can be found as follows:

Combo operands 0 through 3 represent literal values 0 through 3.
Combo operand 4 represents the value of register A.
Combo operand 5 represents the value of register B.
Combo operand 6 represents the value of register C.
Combo operand 7 is reserved and will not appear in valid programs.
*/
Integer extractValueFromCombo(const Context& r, Integer combo) {
    switch (combo) {
        case 0:
        case 1:
        case 2:
        case 3:
            return combo;
        case 4:
            return r.a;
        case 5:
            return r.b;
        case 6:
            return r.c;
        default:
            std::stringstream ss;
            ss << "Invalid combo operand " << combo;
            throw std::runtime_error(ss.str());
    }
}

/*
The adv instruction (opcode 0) performs division.
The numerator is the value in the A register.
The denominator is found by raising 2 to the power of the instruction's combo operand.
(So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.)
The result of the division operation is truncated to an integer and then written to the A register.
*/
void adv(Context& r, Integer operand) {
    r.a >>= extractValueFromCombo(r, operand);
    r.ip += 2;
}
REGISTER_CALL(adv, 0);


/*
The bxl instruction (opcode 1) calculates the bitwise XOR of register B
and the instruction's literal operand, then stores the result in register B.
*/
void bxl(Context& r, Integer operand) {
    r.b ^= operand;
    r.ip += 2;
}
REGISTER_CALL(bxl, 1);

/*
The bst instruction (opcode 2) calculates the value of its combo operand modulo 8
(thereby keeping only its lowest 3 bits), then writes that value to the B register.
*/
void bst(Context& r, Integer operand) {
    r.b = extractValueFromCombo(r, operand) & 7;
    r.ip += 2;
}
REGISTER_CALL(bst, 2);

/*
The jnz instruction (opcode 3) does nothing if the A register is 0.
However, if the A register is not zero, it jumps by setting the instruction pointer
to the value of its literal operand; if this instruction jumps,
the instruction pointer is not increased by 2 after this instruction.
*/
void jnz(Context& r, Integer operand) {
    if (r.a) {
        r.ip = operand;
    } else {
        r.ip += 2;
    }
}
REGISTER_CALL(jnz, 3);

/*
The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C,
then stores the result in register B. (For legacy reasons,
this instruction reads an operand but ignores it.)
*/
void bxc(Context& r, Integer /*ignored*/) {
    r.b ^= r.c;
    r.ip += 2;
}
REGISTER_CALL(bxc, 4);

/*
The out instruction (opcode 5) calculates the value of its combo operand modulo 8,
then outputs that value. (If a program outputs multiple values, they are separated by commas.)
*/
void out(Context& r, Integer operand) {
    std::cout << (extractValueFromCombo(r, operand) & 7) << ",";
    r.ip += 2;
}
REGISTER_CALL(out, 5);

/*
The bdv instruction (opcode 6) works exactly like the adv instruction except
that the result is stored in the B register. (The numerator is still read from the A register.)
*/
void bdv(Context& r, Integer operand) {
    r.b = r.a >> extractValueFromCombo(r, operand);
    r.ip += 2;
}
REGISTER_CALL(bdv, 6);

/*
The cdv instruction (opcode 7) works exactly like
the adv instruction except that the result is stored in the C register.
(The numerator is still read from the A register.)
*/
void cdv(Context& r, Integer operand) {
    r.c = r.a >> extractValueFromCombo(r, operand);
    r.ip += 2;
}
REGISTER_CALL(cdv, 7);

}

class Interpreter {
public:
    explicit Interpreter(Context rs) : context_(rs) {}

    void run(const Program& program) {
        while (static_cast<size_t>(context_.ip) < program.size()) {
            int opcode = program[context_.ip];
            int operand = program.at(context_.ip + 1);
            ops::opcode2call[opcode](context_, operand);
        }
        std::cout << std::endl << "!!! Program halted !!!" << std::endl;
    }
private:
    Context context_;
};

Program parseProgram(std::string_view sv) {
    Program p;

    auto parts = utils::split(sv, ": ");
    auto parts2 = utils::split(parts.at(1), ',');

    std::transform(parts2.begin(), parts2.end(), std::back_inserter(p), utils::parse<Integer>);

    return p;
}

}

int main() {
    auto lines = utils::readLines("input.txt");

    Context rs;
    rs.a = parseRegister(lines.at(0), 'A');
    rs.b = parseRegister(lines.at(1), 'B');
    rs.c = parseRegister(lines.at(2), 'C');
    assert(lines.at(3).empty());

    Interpreter i(rs);

    Program p = parseProgram(lines.at(4));
    // std::cout << "Program: " << p << std::endl;

    // 7,0,0,2,0,7,3,2,2

    i.run(p);

    return 0;
}