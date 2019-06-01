#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class LIS : public RFormat {
  public:
    LIS(unsigned int d) : RFormat{0, 0, d, 0b010100} {}
    LIS(LIS const &rhs) = default;
    LIS(LIS &&rhs) = default;
    ~LIS() = default;

    LIS &operator=(LIS const &rhs) = default;
    LIS &operator=(LIS &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<LIS>(*this); }
    Instruction clone_inst() override {return std::make_unique<LIS>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "lis", d_, -1, -1);
    }
};

inline void LIS::exec(Machine &machine) {
    machine.add_undo([]() {});

    int pc = machine.get_pc();
    int literal = machine.get_mem(pc);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, d = d_,
         d_val = machine.register_[d_]->clone_inst().release(),
         pc = machine.program_counter_->clone_inst().release()]() {
            machine.register_[d] = Instruction{d_val};
            machine.program_counter_ = Instruction{pc};
        };
    machine.add_undo(undo);

    machine.set_pc(pc + 4, false);
    machine.set_reg(d_, literal);
}
