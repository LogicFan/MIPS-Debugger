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
    int low = machine.get_low();
    machine.set_reg(d_, low);
}
