#pragma once

#include "../Instruction_base.hpp"

class word : public Instruction_base {
  private:
    const int word_;
    const unsigned long from_;

  public:
    word(int word, unsigned long from) : word_{word}, from_{from} {}
    word(word const &rhs) = default;
    word(word &&rhs) = default;
    ~word() = default;

    word &operator=(word const &rhs) = default;
    word &operator=(word &&rhs) = default;

    int to_binary() override { return word_; }

    void exec(Machine &machine) override {
        std::cout << "Warning : Executing word instruction!";
        std::cout << std::endl;
        Instruction_base::parse(word_)->exec(machine);
    }

    Data clone() override { return std::make_unique<word>(*this); }
    Instruction clone_inst() override { return std::make_unique<word>(*this); }

    std::ostream &print(std::ostream &out) override {
        out << ".word " << std::hex << word_;
        std::cout << std::dec;
    }

    friend int main(int argc, char *argv[]);
};