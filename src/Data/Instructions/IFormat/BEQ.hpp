#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "IFormat.hpp"

extern std::map<std::string, size_t> symbol_table;

class BEQ : public IFormat {
    std::string label_;

  public:
    BEQ(unsigned int s, unsigned int t, unsigned int i)
        : IFormat{0b000100, s, t, i} {}
    BEQ(unsigned int s, unsigned int t, std::string label)
        : IFormat{0b000101, s, t, 0}, label_{label} {} // Need to change i
    BEQ(BEQ const &rhs) = default;
    BEQ(BEQ &&rhs) = default;
    ~BEQ() = default;

    BEQ &operator=(BEQ const &rhs) = default;
    BEQ &operator=(BEQ &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<BEQ>(*this); }

    std::ostream &print(std::ostream &out) override {
        return IFormat::print(out, "beq", label_);
    }

    void resolve_symbol(size_t self) {
        if (label_ != "") {
            // This is initialized by label
            size_t label = symbol_table.at(label_);
            i_ = static_cast<short>((label - self) / 4 - 1);
        }
    }
};

inline void BEQ::exec(Machine &machine) {
    machine.add_undo([](){});

    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);
    int pc = machine.get_pc();
    int offset = static_cast<short>(i_);

    machine.undo_stack_.pop();
    std::function<void()> undo =
        [&machine = machine,
         pc = machine.program_counter_->clone_inst().release()]() {
            machine.program_counter_ = Instruction{pc};
        };
    machine.add_undo(undo);

    if (reg_s_ == reg_t_) {
        machine.set_pc(pc + offset * 4);
    }
}
