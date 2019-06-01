#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class ADD : public RFormat {
  public:
    ADD(unsigned int d, unsigned int s, unsigned int t)
        : RFormat{s, t, d, 0b100000} {}
    ADD(ADD const &rhs) = default;
    ADD(ADD &&rhs) = default;
    ~ADD() = default;

    ADD &operator=(ADD const &rhs) = default;
    ADD &operator=(ADD &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<ADD>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "add", d_, s_, t_);
    }
};

inline void ADD::exec(Machine &machine) {
    machine.add_undo([]() {});

    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, d = d_,
         d_val = machine.register_[d_]->clone_inst().release()] {
            machine.register_[d] = Instruction{d_val};
        };
    machine.add_undo(undo);

    machine.set_reg(d_, reg_s_ + reg_t_);
}
