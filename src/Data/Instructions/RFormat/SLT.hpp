#pragma once

#include "RFormat.hpp"
#include "../../../Machine.hpp"
#include "../Word/word.hpp"

class SLT : public RFormat {
  public:
    SLT(unsigned int d, unsigned int s, unsigned int t) : RFormat{s, t, d, 0b101010} {}
    SLT(SLT const &rhs) = default;
    SLT(SLT &&rhs) = default;
    ~SLT() = default;

    SLT &operator=(SLT const &rhs) = default;
    SLT &operator=(SLT &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SLT>(*this); }
    Instruction clone_inst() override {return std::make_unique<SLT>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "slt", d_, s_, t_);
    }
};

inline void SLT::exec(Machine &machine) {
    machine.add_undo([](){});

    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, d = d_,
         d_val = machine.register_[d_]->clone_inst().release()] {
            machine.register_[d] = Instruction{d_val};
        };
    machine.add_undo(undo);

    if(reg_s_ < reg_t_) {
        machine.set_reg(d_, 1);
    } else {
        machine.set_reg(d_, 0);
    }
}