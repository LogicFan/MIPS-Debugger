#pragma once

#include "../Data_base.hpp"
#include <iomanip>

struct Instruction_base : public Data_base {
  protected:
    int file_line_;

  public:
    bool break_;

    Instruction_base() : file_line_{-1}, break_{false} {}
    Instruction_base(Instruction_base const &rhs) = default;
    Instruction_base(Instruction_base &&rhs) = default;
    ~Instruction_base() = default;

    Instruction_base &operator=(Instruction_base const &rhs) = default;
    Instruction_base &operator=(Instruction_base &&rhs) = default;

    void set_break(bool _break) {
        break_ = _break;
        return;
    }
    void set_file_line(bool file_line) {
        file_line_ = file_line;
        return;
    }

    virtual int to_binary() = 0;
    virtual void exec(Machine &machine) = 0;

    static Instruction parse(int binary);

    virtual Instruction clone_inst() = 0;
};

inline std::ostream &operator<<(std::ostream &out, Instruction const &inst) {
    return inst->print(out);
}
