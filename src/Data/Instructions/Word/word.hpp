#pragma once

#include "../Instruction_base.hpp"
#include <map>

extern std::map<std::string, size_t> symbol_table;

class word : public Instruction_base {
  private:
    int word_;
    std::string label_;
    const unsigned long from_;

  public:
    word(int word, unsigned long from) : word_{word}, from_{from} {}
    word(std::string &label) : label_{label}, from_{0} {}

    word(word const &rhs) = default;
    word(word &&rhs) = default;
    ~word() = default;

    word &operator=(word const &rhs) = default;
    word &operator=(word &&rhs) = default;

    int to_binary(bool warn) override { return word_; }

    void exec(Machine &machine) override {
        std::cout << "Warning : Executing word instruction!";
        std::cout << std::endl;
        Instruction_base::parse(word_)->exec(machine);
    }

    Data clone() override { return std::make_unique<word>(*this); }
    Instruction clone_inst() override { return std::make_unique<word>(*this); }

    std::ostream &print(std::ostream &out) override {
        out << ".word "
            << "0x" << std::setw(8) << std::hex << std::setfill('0') << word_;
        std::cout << std::dec;
        std::cout << std::setfill(' ');
        if(label_ != "") {
            out << "[" << label_ << "]";
        }
        return out;
    }

    void resolve_symbol(size_t self) override {
        if (label_ != "") {
            word_ = symbol_table[label_];
        }
    }
    friend void word_info(Instruction_base *reg);
};