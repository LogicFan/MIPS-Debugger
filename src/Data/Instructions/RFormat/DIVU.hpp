#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class DIVU : public RFormat {
  public:
    DIVU(unsigned int s, unsigned int t) : RFormat{s, t, 0, 0b011011} {}
    DIVU(DIVU const &rhs) = default;
    DIVU(DIVU &&rhs) = default;
    ~DIVU() = default;

    DIVU &operator=(DIVU const &rhs) = default;
    DIVU &operator=(DIVU &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<DIVU>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "divu", -1, s_, t_);
    }
};

inline void DIVU::exec(Machine &machine) {
    unsigned int reg_s_ = static_cast<unsigned int>(machine.get_reg(s_));
    unsigned int reg_t_ = static_cast<unsigned int>(machine.get_reg(t_));

    machine.set_high(static_cast<int>(reg_s_ % reg_t_));
    machine.set_low(static_cast<int>(reg_s_ / reg_t_));
}
