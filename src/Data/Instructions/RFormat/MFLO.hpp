#pragma once

#include "RFormat.hpp"
#include "../../../Machine.hpp"
#include "../Word/word.hpp"

class MFLO : public RFormat {
  public:
    MFLO(unsigned int d) : RFormat{0, 0, d, 0b010010} {}
    MFLO(MFLO const &rhs) = default;
    MFLO(MFLO &&rhs) = default;
    ~MFLO() = default;

    MFLO &operator=(MFLO const &rhs) = default;
    MFLO &operator=(MFLO &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<MFLO>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "mflo", d_, -1, -1);
    }
};

inline void MFLO::exec(Machine &machine) {
    machine.add_undo([](){});

    int low = machine.get_low();

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine, d = d_,
         d_val = machine.register_[d_]->clone_inst().release()]() {
            machine.register_[d] = Instruction{d_val};
        };
    machine.add_undo(undo);

    machine.set_reg(d_, low);
}
