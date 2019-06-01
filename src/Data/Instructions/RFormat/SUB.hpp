#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class SUB : public RFormat {
  public:
    SUB(unsigned int d, unsigned int s, unsigned int t)
        : RFormat{s, t, d, 0b100010} {}
    SUB(SUB const &rhs) = default;
    SUB(SUB &&rhs) = default;
    ~SUB() = default;

    SUB &operator=(SUB const &rhs) = default;
    SUB &operator=(SUB &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SUB>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "sub", d_, s_, t_);
    }
};

inline void SUB::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);

    machine.set_reg(d_, reg_s_ - reg_t_);
}
