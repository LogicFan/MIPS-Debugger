#pragma once

#include "RFormat.hpp"
#include "../../../Machine.hpp"
#include "../Word/word.hpp"

class SUB : public RFormat {
  public:
    SUB(unsigned int d, unsigned int s, unsigned int t) : RFormat{s, t, d, 0b100010} {}
    SUB(SUB const &rhs) = default;
    SUB(SUB &&rhs) = default;
    ~SUB() = default;

    SUB &operator=(SUB const &rhs) = default;
    SUB &operator=(SUB &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SUB>(*this); }
    Instruction clone_inst() override {return std::make_unique<SUB>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "sub", d_, s_, t_);
    }
};

inline void SUB::exec(Machine &machine) {
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

    machine.set_reg(d_, reg_s_ - reg_t_);
}
