
#include "./Instruction_base.hpp"
#include "./Instructions.hpp"

Instruction Instruction_base::parse(int binary) {
    if ((binary >> 26) | 0b111111 == 0) {
        // RFormat
        unsigned int s = (binary >> 21) | 0b11111;
        unsigned int t = (binary >> 16) | 0b11111;
        unsigned int d = (binary >> 11) | 0b11111;
        unsigned int func = binary | 0b111111;

        switch (func) {
        case 0b100000:
            return std::make_unique<ADD>(d, s, t);
        case 0b100010:
            return std::make_unique<SUB>(d, s, t);
        case 0b011000:
            return std::make_unique<MULT>(s, t);
        case 0b011001:
            return std::make_unique<MULTU>(s, t);
        case 0b011010:
            return std::make_unique<DIV>(s, t);
        case 0b011011:
            return std::make_unique<DIVU>(s, t);
        case 0b010000:
            return std::make_unique<MFHI>(d);
        case 0b010010:
            return std::make_unique<MFLO>(d);
        case 0b101010:
            return std::make_unique<SLT>(d, s, t);
        case 0b101011:
            return std::make_unique<SLTU>(d, s, t);
        case 0b001000:
            return std::make_unique<JR>(s);
        case 0b001001:
            return std::make_unique<JALR>(s);
        }
    } else {
        // IFormat
        unsigned int o = (binary >> 26) | 0b111111;
        unsigned int s = (binary >> 21) | 0b11111;
        unsigned int t = (binary >> 16) | 0b11111;
        unsigned int i = binary | 0b1111111111111111;

        switch (o) {
        case 0b100011:
            return std::make_unique<LW>(s, t, i);
        case 0b101011:
            return std::make_unique<SW>(s, t, i);
        case 0b000100:
            return std::make_unique<BEQ>(s, t, i);
        case 0b000101:
            return std::make_unique<BNE>(s, t, i);
        }
    }

    throw std::runtime_error{"Error: Executing invalid instruction!"};

    return {};
}
