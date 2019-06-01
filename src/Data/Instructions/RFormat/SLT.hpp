#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class SLT : public RFormat {
  public:
    SLT(unsigned int d, unsigned int s, unsigned int t)
        : RFormat{s, t, d, 0b101010} {}
    SLT(SLT const &rhs) = default;
    SLT(SLT &&rhs) = default;
    ~SLT() = default;

    SLT &operator=(SLT const &rhs) = default;
    SLT &operator=(SLT &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SLT>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "slt", d_, s_, t_);
    }
};

inline void SLT::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);

    if (reg_s_ < reg_t_) {
        machine.set_reg(d_, 1);
    } else {
        machine.set_reg(d_, 0);
    }
}