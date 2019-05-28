#pragma once
#include <functional>
#include <map>
#include <stack>
#include <vector>

#include "./Data/Data_base.hpp"

struct Machine final {
    mutable Instruction register_[32];
    mutable std::map<size_t, Instruction> memory_;
    mutable Instruction high_;
    mutable Instruction low_;
    mutable Instruction program_counter_;

    std::stack<std::function<void()>> undo_stack_;

    bool break_;
    unsigned long counter_;
    size_t code_area_;

    Machine();
    Machine(Machine const &rhs) = delete;
    Machine(Machine &&rhs) = delete;
    ~Machine() = default;

    Machine &operator=(Machine const &rhs) = delete;
    Machine &operator=(Machine &&rhs) = delete;

    void set_reg(size_t reg, int value, bool warn = true);
    int get_reg(size_t reg, bool warn = true);
    void set_mem(size_t mem, int value, bool warn = true);
    int get_mem(size_t, bool warn = true);
    void set_high(int value, bool warn = true);
    int get_high(bool warn = true);
    void set_low(int value, bool warn = true);
    int get_low(bool warn = true);
    void set_pc(int value, bool warn = true);
    int get_pc(bool warn = true);

    void set_break();

    void add_undo(std::function<void()> const &undo);

    void load(std::vector<Instruction> &&code);
    void exec(Instruction const &inst);

    void pervious();
    bool next();
    // void start();
    // void roll_back(size_t counter_id);

    friend void machine_test();
};

// Done [Error: Accessing invalid register!] machine
// Done [Error: Accessing non-initialized memory!] machine
// Done [Error: Accessing non-aligned memory address!] machine
// Done [Error: Accessing non-initialized register!] machine
// Done [Error: Executing invalid instruction!] Instruction_base::parse

// Done [Warning: Modifying register 0!]
// Done [Warning: Jumping to non-label address!] machine
// Done [Warning: Jumping to non-code area!] machine
// Done [Warning : Executing word instruction!] word
// Done [Warning: Arithmetic operation on non-word instruction!] IFormat, RFormat
// Done [Warning: Modifying code area!] machine


// [Error: Dividing zero!] div, divu

// Break point
// Trace back data
// Print warning or error file line number
// Previous Step and Next Step
// Resume