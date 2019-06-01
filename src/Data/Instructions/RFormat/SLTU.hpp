#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class SLTU : public RFormat {
  public:
    SLTU(unsigned int d, unsigned int s, unsigned int t) : RFormat{s, t, d, 0b101011} {}
    SLTU(SLTU const &rhs) = default;
    SLTU(SLTU &&rhs) = default;
    ~SLTU() = default;

    SLTU &operator=(SLTU const &rhs) = default;
    SLTU &operator=(SLTU &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SLTU>(*this); }
    Instruction clone_inst() override {return std::make_unique<SLTU>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "sltu", d_, s_, t_);
    }
};

inline void SLTU::exec(Machine &machine) {
    machine.add_undo([](){});

    unsigned int reg_s_ = static_cast<unsigned int>(machine.get_reg(s_));
    unsigned int reg_t_ = static_cast<unsigned int>(machine.get_reg(t_));

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, d = d_,
         d_val = machine.register_[d_]->clone_inst().release()] {
            machine.register_[d] = Instruction{d_val};
        };
    machine.add_undo(undo);

    if (reg_s_ < reg_t_) {
        machine.set_reg(d_, 1);
    } else {
        machine.set_reg(d_, 0);
    }
}
