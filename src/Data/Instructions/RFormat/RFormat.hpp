#pragma once

#include "../Instruction_base.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

class RFormat : public Instruction_base {
  protected:
    unsigned int s_;
    unsigned int t_;
    unsigned int d_;
    unsigned int func_;

  public:
    RFormat(unsigned int s, unsigned int t, unsigned int d, unsigned int func)
        : s_{s}, t_{t}, d_{d}, func_{func} {
        assert(s_ < 32);
        assert(t_ < 32);
        assert(d_ < 32);
        assert(func_ < 64);
    }
    RFormat(RFormat const &rhs) = default;
    RFormat(RFormat &&rhs) = default;
    ~RFormat() = default;

    RFormat &operator=(RFormat const &rhs) = default;
    RFormat &operator=(RFormat &&rhs) = default;

    int to_binary() override {
        std::cout << "Warning: Arithmetic operation on non-word instruction!"
                  << std::endl;
        return func_ | (d_ << 11) | (t_ << 16) | (s_ << 21);
    }

    std::ostream &print(std::ostream &out, std::string inst, int d, int s,
                        int t) {
        out << std::left << std::setw(5) << inst;
        if (d == -1) {
            out << "   ";
        } else {
            out << "$" << std::setw(2) << d;
        }

        if (d == -1 || s == -1) {
            out << " ";
        } else {
            out << ",";
        }

        if (s == -1) {
            out << "   ";
        } else {
            out << "$" << std::setw(2) << s;
        }

        if (s == -1 || t == -1) {
            out << " ";
        } else {
            out << ",";
        }

        if (t == -1) {
            out << "   ";
        } else {
            out << "$" << std::setw(2) << t;
        }

        std::cout << std::right;

        return out;
    }
};
