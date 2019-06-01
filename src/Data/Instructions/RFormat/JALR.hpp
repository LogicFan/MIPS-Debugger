#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class JALR : public RFormat {
  public:
    JALR(unsigned int s) : RFormat{s, 0, 0, 0b001001} {}
    JALR(JALR const &rhs) = default;
    JALR(JALR &&rhs) = default;
    ~JALR() = default;

    JALR &operator=(JALR const &rhs) = default;
    JALR &operator=(JALR &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<JALR>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "jalr", -1, s_, -1);
    }
};

inline void JALR::exec(Machine &machine) {
    machine.add_undo([]() {});

    int reg_s_ = machine.get_reg(s_);
    int pc = machine.get_pc();

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, s = s_,
         s_val = machine.register_[s_]->clone_inst().release(),
         pc = machine.program_counter_->clone_inst().release()]() {
            machine.register_[s] = Instruction{s_val};
            machine.program_counter_ = Instruction{pc};
        };
    machine.add_undo(undo);

    machine.set_pc(reg_s_);
    machine.set_reg(31, pc);
}
