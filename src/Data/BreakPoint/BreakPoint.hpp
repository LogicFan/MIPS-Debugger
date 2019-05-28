#pragma once

#include "../Data_base.hpp"
#include <stdexcept>

class BreakPoint : public Data_base {
  public:
    BreakPoint() = default;
    BreakPoint(BreakPoint const &rhs) = default;
    BreakPoint(BreakPoint &&rhs) = default;
    ~BreakPoint() = default;

    BreakPoint &operator=(BreakPoint const &rhs) = default;
    BreakPoint &operator=(BreakPoint &&rhs) = default;

    Data clone() override;

    std::ostream &print(std::ostream &out) override;
};

inline Data BreakPoint::clone() { return std::make_unique<BreakPoint>(*this); }

inline std::ostream &BreakPoint::print(std::ostream &out) {
    return out << "Break point";
}