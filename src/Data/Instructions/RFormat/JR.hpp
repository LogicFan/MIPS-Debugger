#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class JR : public RFormat {
  public:
    JR(unsigned int s) : RFormat{s, 0, 0, 0b001000} {}
    JR(JR const &rhs) = default;
    JR(JR &&rhs) = default;
    ~JR() = default;

    JR &operator=(JR const &rhs) = default;
    JR &operator=(JR &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<JR>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "jr", -1, s_, -1);
    }
};

inline void JR::exec(Machine &machine) {
    machine.add_undo([]() {});

    int reg_s_ = machine.get_reg(s_);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine,
         pc = machine.program_counter_->clone_inst().release()]() {
            machine.program_counter_ = Instruction{pc};
        };
    machine.add_undo(undo);

    machine.set_pc(reg_s_);
}