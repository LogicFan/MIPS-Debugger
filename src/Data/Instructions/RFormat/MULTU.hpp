#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class MULTU : public RFormat {
  public:
    MULTU(unsigned int s, unsigned int t) : RFormat{s, t, 0, 0b011001} {}
    MULTU(MULTU const &rhs) = default;
    MULTU(MULTU &&rhs) = default;
    ~MULTU() = default;

    MULTU &operator=(MULTU const &rhs) = default;
    MULTU &operator=(MULTU &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<MULTU>(*this); }
    Instruction clone_inst() override {return std::make_unique<MULTU>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "multu", -1, s_, t_);
    }
};

inline void MULTU::exec(Machine &machine) {
    machine.add_undo([](){});

    unsigned int reg_s_ = static_cast<unsigned int>(machine.get_reg(s_));
    unsigned int reg_t_ = static_cast<unsigned int>(machine.get_reg(t_));
    unsigned long temp =
        static_cast<unsigned long>(reg_s_) * static_cast<unsigned long>(reg_t_);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, high = machine.high_->clone_inst().release(),
         low = machine.low_->clone_inst().release()]() {
            machine.high_ = Instruction{high};
            machine.low_ = Instruction{low};
        };
    machine.add_undo(undo);

    machine.set_high(reinterpret_cast<int *>(&temp)[0]);
    machine.set_low(reinterpret_cast<int *>(&temp)[1]);
}