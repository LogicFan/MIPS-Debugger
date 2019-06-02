#include "Machine.hpp"
#include "./Data/Instructions/RFormat/LIS.hpp"
#include "./Data/Instructions/Word/word.hpp"
#include <set>

extern std::set<size_t> labeled_address;

bool Machine::break_ = false;
unsigned long Machine::counter_ = 1;
size_t Machine::code_area_ = 0;
std::stack<std::function<void()>> Machine::undo_stack_;

Machine::Machine() {
    // $0 is hardware 0
    register_[0] = std::make_unique<word>(0, 0);
    // termination program counter value is -1 (convention)
    register_[31] = std::make_unique<word>(-1, 0);
    // set stack pointer
    register_[30] = std::make_unique<word>(-2147483648, 0);

    // set pc = 0
    program_counter_ = std::make_unique<word>(0, 0);
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

int Machine::get_reg(size_t reg, bool warn) const {
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

int Machine::get_mem(size_t mem, bool warn) const {
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

int Machine::get_high(bool warn) const {
    if (!high_) {
        throw std::runtime_error{"Error: Accessing non-initialized register!"};
    }

    return high_->to_binary();
}

void Machine::set_low(int value, bool warn) {
    low_ = std::make_unique<word>(value, counter_);
}

int Machine::get_low(bool warn) const {
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

int Machine::get_pc(bool warn) const {
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
    int pc = get_pc();
    set_pc(pc + 4, false);

    inst->exec(*this);

    ++counter_;
}

void Machine::pervious() {
    if (counter_ > 1) {
        --counter_;

        undo_stack_.top()();
        undo_stack_.pop();

    } else {
        std::cout << "Error: There is no pervious steps!" << std::endl;
    }
}

bool Machine::next(bool resume, bool print) {
    int pc = get_pc();

    if (pc == -1) {
        // MIPS program terminated
        return false;
    }

    Instruction &next_inst = memory_[pc];
    if (!next_inst) {
        throw std::runtime_error{"Error: Accessing non-initialized memory!"};
    }

    if (next_inst->break_ && !resume) {
        // This is a break point
        return false;
    }

    if (print) {
        std::cout << "[" << std::setw(20) << std::setfill('0') << counter_
                  << "] ";
        std::cout << std::setfill(' ');
        std::cout << next_inst;
        if (dynamic_cast<LIS *>(next_inst.get())) {
            // This is a lis instruction
            Instruction &literel = memory_[pc + 4];
            if (!literel) {
                throw std::runtime_error{
                    "Error: Accessing non-initialized memory!"};
            }
            std::cout << "[" << literel << "]";
        }

        std::cout << std::endl;
    }

    std::function<void()> undo = [machine_src = *this,
                                  &machine_targ = *this]() {
        machine_targ = machine_src;
    };

    add_undo(undo);

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

Machine::Machine(Machine const &rhs) {
    for (int i = 0; i < 32; ++i) {
        if (rhs.register_[i]) {
            this->register_[i] = rhs.register_[i]->clone_inst();
        }
    }

    if (rhs.high_) {
        this->high_ = rhs.high_->clone_inst();
    }

    if (rhs.low_) {
        this->low_ = rhs.low_->clone_inst();
    }

    if (rhs.program_counter_) {
        this->program_counter_ = rhs.program_counter_->clone_inst();
    }

    for (auto const &inst : rhs.memory_) {
        if (inst.second) {
            this->memory_[inst.first] = inst.second->clone_inst();
        }
    }
}

Machine &Machine::operator=(Machine const &rhs) {
    this->~Machine();
    new (this) Machine{rhs};
    return *this;
}

bool Machine::roll_back(size_t counter_id) {
    if (counter_id == 0 || counter_id >= counter_) {
        std::cout << "Error: Cannot rollback to ";
        std::cout << "[" << std::setw(20) << std::setfill('0')
                  << counter_id << "] ";
        std::cout << std::dec;
        std::cout << std::setfill(' ');
        std::cout << "!" << std::endl;
        return false;
    }

    while (counter_id != counter_) {
        pervious();
    }

    std::cout << "Back to ";
    next(true);
    return true;
}