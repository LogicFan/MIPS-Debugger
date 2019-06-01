#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "IFormat.hpp"

class LW : public IFormat {
  public:
    LW(unsigned int s, unsigned int t, unsigned int i)
        : IFormat{0b000100, s, t, i} {}
    LW(LW const &rhs) = default;
    LW(LW &&rhs) = default;
    ~LW() = default;

    LW &operator=(LW const &rhs) = default;
    LW &operator=(LW &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<LW>(*this); }
    Instruction clone_inst() override { return std::make_unique<LW>(*this); }

    std::ostream &print(std::ostream &out) override {
        out << std::left << std::setw(5) << "lw";
        out << "$" << std::setw(2) << t_ << ",";
        out << std::setw(6) << static_cast<short>(i_) << "(";
        out << "$" << std::setw(2) << s_ << ")";
        out << std::right;
        return out;
    }
};

inline void LW::exec(Machine &machine) {
    long reg_s_ = static_cast<unsigned>(machine.get_reg(s_));
    int offset = static_cast<short>(i_);
    int value = machine.get_mem(reg_s_ + offset);

    machine.set_reg(t_, value);
}
