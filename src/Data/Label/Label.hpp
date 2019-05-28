#pragma once

#include "../Data_base.hpp"
#include <stdexcept>
#include <string>

class Label : public Data_base {
    std::string label_;
  public:
    Label(std::string const &label) : label_{label} {}
    Label(Label const &rhs) = default;
    Label(Label &&rhs) = default;
    ~Label() = default;

    Label &operator=(Label const &rhs) = default;
    Label &operator=(Label &&rhs) = default;

    Data clone() override;

    std::string const &get_label() {
        return label_;
    }

    std::ostream &print(std::ostream &out) override;
};

inline Data Label::clone() { return std::make_unique<Label>(*this); }

inline std::ostream &Label::print(std::ostream &out) {
    return out << label_ << ":";
}