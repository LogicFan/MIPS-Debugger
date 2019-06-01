#include "Data/BreakPoint/BreakPoint.hpp"
#include "Data/Data_base.hpp"
#include "Data/Instructions/Instruction_base.hpp"
#include "Data/Instructions/Instructions.hpp"
#include "Data/Label/Label.hpp"
#include "Machine.hpp"
#include "Parser/Parser.hpp"
#include <fstream>
#include <iostream>

void word_info(Instruction_base *reg);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Version 0.1 (Beta)" << std::endl;
        return 0;
    }

    std::ifstream code_file{argv[1]};
    std::vector<Instruction> program = parse(code_file);

    Machine machine;
    machine.load(std::move(program));

RESUME:
    while (machine.next()) {
    };

    std::cout << machine << std::endl;

    while (true) {

        std::string str;
        std::cin >> str;

        if (str == "exit") {
            return 0;
        } else if (str == "next") {
            machine.next(true);
            std::cout << machine << std::endl;
        } else if (str == "previous") {
            machine.pervious();
            std::cout << machine << std::endl;
        } else if (str == "resume") {
            machine.next(true);
            goto RESUME;
        } else if (str == "register") {
            std::string reg;
            std::cin >> reg;
            if (reg == "pc") {
                std::cout << "$pc: ";
                word_info(machine.program_counter_.get());
            } else if (reg == "hi") {
                std::cout << "$hi: ";
                word_info(machine.high_.get());
            } else if (reg == "lo") {
                std::cout << "$lo: ";
                word_info(machine.low_.get());
            } else {
                std::stringstream ss{reg};
                int reg_i;
                ss >> reg_i;
                std::cout << "$";
                std::cout << std::setw(2) << std::left << reg_i;
                std::cout << std::right;
                std::cout << ": ";
                word_info(machine.register_[reg_i].get());
            }
        } else if (str == "memory") {
            size_t mem;
            std::cin >> mem;
            if (mem % 4 == 0) {
                std::cout << "0x" << std::setw(8) << std::hex
                          << std::setfill('0') << mem;
                std::cout << std::dec;
                std::cout << std::setfill(' ');

                std::cout << ": ";
                word_info(machine.memory_[mem].get());
            } else {
                std::cout << "Error: Accessing non-aligned memory address!"
                          << std::endl;
            }
        } else if (str == "rollback") {
            std::cout << machine << std::endl;
        }
    }

    return 0;
}

void word_info(Instruction_base *reg) {
    if (reg) {
        std::cout << std::bitset<32>{
            static_cast<unsigned int>(reg->to_binary())};
        std::cout << "  from: ";
        word *w = dynamic_cast<word *>(reg);
        if (w && w->from_ > 0) {
            std::cout << "[" << std::hex << std::setw(20) << std::setfill('0')
                      << w->from_ << "] ";
            std::cout << std::dec;
            std::cout << std::setfill(' ');
        } else {
            std::cout << "nowhere";
        }
    } else {
        std::cout << "null                            ";
        std::cout << "  from: nowhere";
    }

    std::cout << std::endl;
}
