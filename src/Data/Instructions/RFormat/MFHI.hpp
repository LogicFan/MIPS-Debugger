#pragma once

#include "../../../Machine.hpp"
#include "../Word/word.hpp"
#include "RFormat.hpp"

class MFHI : public RFormat {
  public:
    MFHI(unsigned int d) : RFormat{0, 0, d, 0b010000} {}
    MFHI(MFHI const &rhs) = default;
    MFHI(MFHI &&rhs) = default;
    ~MFHI() = default;

    MFHI &operator=(MFHI const &rhs) = default;
    MFHI &operator=(MFHI &&rhs) = default;

    void exec(Machine &machine) override;

    Data clone() override { return std::make_unique<MFHI>(*this); }

    std::ostream &print(std::ostream &out) override {
        return RFormat::print(out, "mfhi", d_, -1, -1);
    }
};

inline void MFHI::exec(Machine &machine) {
    int high = machine.get_high();

    machine.set_reg(d_, high);
}