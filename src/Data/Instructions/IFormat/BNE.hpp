#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "IFormat.hpp"

extern std::map<std::string, size_t> symbol_table;

class BNE : public IFormat {
    std::string label_;

  public:
    BNE(unsigned int s, unsigned int t, unsigned int i)
        : IFormat{0b000101, s, t, i} {}
    BNE(unsigned int s, unsigned int t, std::string label)
        : IFormat{0b000101, s, t, 0}, label_{label} {} // Need to change i
    BNE(BNE const &rhs) = default;
    BNE(BNE &&rhs) = default;
    ~BNE() = default;

    BNE &operator=(BNE const &rhs) = default;
    BNE &operator=(BNE &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<BNE>(*this); }
    Instruction clone_inst() override { return std::make_unique<BNE>(*this); }

    std::ostream &print(std::ostream &out) override {
        out << std::left << std::setw(5) << "bne";
        out << "$" << std::setw(2) << s_ << ",";
        out << "$" << std::setw(2) << t_ << ",";
        out << std::setw(6) << static_cast<short>(i_);
        if (label_ != "") {
            out << "[" << label_ << "]";
        }
        out << std::right;
        return out;
    }

    void resolve_symbol(size_t self) override {
        if (label_ != "") {
            // This is initialized by label
            size_t label = symbol_table.at(label_);
            i_ = static_cast<short>((label - self) / 4 - 1);
        }
    }
};

inline void BNE::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);
    int pc = machine.get_pc();
    int offset = static_cast<short>(i_);

    if (reg_s_ != reg_t_) {
        machine.set_pc(pc + offset * 4);
    }
}
