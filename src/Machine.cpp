#include "Machine.hpp"
#include "./Data/Instructions/Word/word.hpp"
#include <set>

extern std::set<size_t> labeled_address;

Machine::Machine() {
    // $0 is hardware 0
    register_[0] = std::make_unique<word>(0, 0);
    // termination program counter value is -1 (convention)
    register_[31] = std::make_unique<word>(-1, 0);
    // set stack pointer
    register_[30] = std::make_unique<word>(-2147483648, 0);

    // set pc = 0
    program_counter_ = std::make_unique<word>(0, 0);

    break_ = false;

    // set counter begin from 1
    counter_ = 1; // setw(5) << setfill('0')
    code_area_ = 0;
}

void Machine::set_reg(size_t reg, int value, bool warn) {
    if (reg >= 32) {
        throw std::runtime_error{"Error: Accessing invalid register!"};
    }

    if (reg == 0) {
        if (warn) {
            std::cout << "Warning: Modifying register 0!" << std::endl;
        }
        return;
    }

    register_[reg] = std::make_unique<word>(value, counter_);
}

int Machine::get_reg(size_t reg, bool warn) {
    if (reg >= 32) {
        throw std::runtime_error{"Error: Accessing invalid register!"};
    } else if (!register_[reg]) {
        throw std::runtime_error{"Error: Accessing non-initialized register!"};
    }

    return register_[reg]->to_binary();
}

void Machine::set_mem(size_t mem, int value, bool warn) {
    if (mem % 4 != 0) {
        throw std::runtime_error{
            "Error: Accessing non-aligned memory address!"};
    }

    if (mem < code_area_ && warn) {
        std::cout << "Warning: Modifying code area!" << std::endl;
    }

    memory_[mem] = std::make_unique<word>(value, counter_);
}

int Machine::get_mem(size_t mem, bool warn) {
    if (mem % 4 != 0) {
        throw std::runtime_error{
            "Error: Accessing non-aligned memory address!"};
    } else if (!memory_[mem]) {
        throw std::runtime_error{"Error: Accessing non-initialized memory!"};
    }

    return memory_[mem]->to_binary();
}

void Machine::set_high(int value, bool warn) {
    high_ = std::make_unique<word>(value, counter_);
}

int Machine::get_high(bool warn) {
    if (!high_) {
        throw std::runtime_error{"Error: Accessing non-initialized register!"};
    }

    return high_->to_binary();
}

void Machine::set_low(int value, bool warn) {
    low_ = std::make_unique<word>(value, counter_);
}

int Machine::get_low(bool warn) {
    if (!low_) {
        throw std::runtime_error{"Error: Accessing non-initialized register!"};
    }

    return low_->to_binary();
}

void Machine::set_pc(int value, bool warn) {
    program_counter_ = std::make_unique<word>(value, counter_);

    if (value != -1 && warn) {
        if (value >= code_area_) {
            std::cout << "Warning: Jumping to non-code area!" << std::endl;
        } else if (labeled_address.count(value) == 0) {
            std::cout << "Warning: Jumping to non-label address!" << std::endl;
        }
    }
}

int Machine::get_pc(bool warn) {
    if (!program_counter_) {
        throw std::runtime_error{"Error: Accessing non-initialized register!"};
    }

    return program_counter_->to_binary();
}

void Machine::set_break() { break_ = true; }

void Machine::add_undo(std::function<void()> const &undo) {
    undo_stack_.push(undo);
}

void Machine::load(std::vector<Instruction> &&code) {
    for (int i = 0; i < code.size(); ++i) {
        memory_[i * 4] = std::move(code[i]);
    }
    code_area_ = code.size() * 4;
}

void Machine::exec(Instruction const &inst) {
    ++counter_;

    int pc = get_pc();
    set_pc(pc + 4, false);

    inst->exec(*this);
}

void Machine::pervious() {
    if(counter_ > 1) {
        --counter_;
        undo_stack_.top()();
        undo_stack_.pop();
        set_pc(get_pc() - 4);
    } else {
        std::cout << "Error: There is no pervious steps!" << std::endl; 
    }
}

bool Machine::next() {
    int pc = get_pc();

    if (pc == -1) {
        return false;
    }

    Instruction &next_inst = memory_[pc];
    if (!next_inst) {
        throw std::runtime_error{"Error: Accessing non-initialized memory!"};
    }

    std::cout << "[" << std::right << std::hex << std::setw(20)
              << std::setfill('0') << counter_ << "] " << std::setfill(' ');
    std::cout << next_inst << std::endl;

    try {
        exec(next_inst);
    } catch (std::runtime_error const &e) {
        std::cout << e.what() << std::endl;
        undo_stack_.top()();
        undo_stack_.pop();
        return false;
    }

    return true;
}
