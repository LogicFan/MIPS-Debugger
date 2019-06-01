#pragma once

#include "IFormat.hpp"
#include "../../../Machine.hpp"
#include "../Word/word.hpp"

class SW : public IFormat {
  public:
    SW(unsigned int s, unsigned int t, unsigned int i) : IFormat{0b000100, s, t, i} {}
    SW(SW const &rhs) = default;
    SW(SW &&rhs) = default;
    ~SW() = default;

    SW &operator=(SW const &rhs) = default;
    SW &operator=(SW &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<SW>(*this); }
    Instruction clone_inst() override { return std::make_unique<SW>(*this); }

    std::ostream &print(std::ostream &out) override {
        return IFormat::print(out, "sw");
    }
};

inline void SW::exec(Machine &machine) {
    int reg_s_ = machine.get_reg(s_);
    int reg_t_ = machine.get_reg(t_);
    int offset = static_cast<short>(i_);
    
    machine.set_mem(reg_s_ + offset, t_);
}
