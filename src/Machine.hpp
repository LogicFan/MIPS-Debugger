#pragma once
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
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

    static std::stack<std::function<void()>> undo_stack_;

    static bool break_;
    static unsigned long counter_;
    static size_t code_area_;

    Machine();
    Machine(Machine const &rhs);
    ~Machine() = default;

    Machine &operator=(Machine const &rhs);

    void set_reg(size_t reg, int value, bool warn = true);
    int get_reg(size_t reg, bool warn = true) const;
    void set_mem(size_t mem, int value, bool warn = true);
    int get_mem(size_t, bool warn = true) const;
    void set_high(int value, bool warn = true);
    int get_high(bool warn = true) const;
    void set_low(int value, bool warn = true);
    int get_low(bool warn = true) const;
    void set_pc(int value, bool warn = true);
    int get_pc(bool warn = true) const;

    void set_break();

    void add_undo(std::function<void()> const &undo);

    void load(std::vector<Instruction> &&code);
    void exec(Instruction const &inst);

    void pervious();
    bool next(bool resume = false);
    // void start();
    // void roll_back(size_t counter_id);

    friend void machine_test();
};

inline std::ostream &operator<<(std::ostream &out, Machine const &machine) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i * 4 + j < 32) {
                std::cout << "$" << std::left << std::setw(2) << i * 4 + j
                          << ": ";
                try {
                    std::cout << std::bitset<32>{
                        static_cast<unsigned int>(machine.get_reg(i * 4 + j))};
                } catch (...) {
                    std::cout << "null                            ";
                }
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::right;

    std::cout << "$hi: ";
    try {
        std::cout << std::bitset<32>{
            static_cast<unsigned int>(machine.get_high())};
    } catch (...) {
        std::cout << "null                            ";
    }

    std::cout << "  ";
    std::cout << "$lo: ";
    try {
        std::cout << std::bitset<32>{
            static_cast<unsigned int>(machine.get_low())};
    } catch (...) {
        std::cout << "null                            ";
    }

    std::cout << "  ";
    std::cout << "$pc: ";
    try {
        std::cout << std::bitset<32>{
            static_cast<unsigned int>(machine.get_pc())};
    } catch (...) {
        std::cout << "null                            ";
    }
    std::cout << std::endl;
}

// Break point
// Trace back data
// Print warning or error file line number
// Previous Step and Next Step
// Resume