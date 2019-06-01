#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class MULT : public RFormat {
  public:
    MULT(unsigned int s, unsigned int t) : RFormat{s, t, 0, 0b011000} {}
    MULT(MULT const &rhs) = default;
    MULT(MULT &&rhs) = default;
    ~MULT() = default;

    MULT &operator=(MULT const &rhs) = default;
    MULT &operator=(MULT &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<MULT>(*this); }
    Instruction clone_inst() override { return std::make_unique<MULT>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "mult", -1, s_, t_);
    }
};

inline void MULT::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);
    long temp = static_cast<long>(reg_s_) * static_cast<long>(reg_t_);

    if (is_small_endian()) {
        machine.set_high(reinterpret_cast<int *>(&temp)[1]);
        machine.set_low(reinterpret_cast<int *>(&temp)[0]);
    } else {
        machine.set_high(reinterpret_cast<int *>(&temp)[0]);
        machine.set_low(reinterpret_cast<int *>(&temp)[1]);
    }
}
