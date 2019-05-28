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

    std::ostream &print(std::ostream &out) override {
        return IFormat::print(out, "lw");
    }
};

inline void LW::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int offset = static_cast<short>(i_);
    int value = machine.get_mem(reg_s_ + offset);

    machine.set_reg(t_, value);
}
