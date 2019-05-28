#include "Data/BreakPoint/BreakPoint.hpp"
#include "Data/Data_base.hpp"
#include "Data/Instructions/Instruction_base.hpp"
#include "Data/Instructions/Instructions.hpp"
#include "Data/Label/Label.hpp"
#include "Machine.hpp"
#include "Parser/Parser.hpp"
#include <bitset>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Version 0.1 (Beta)" << std::endl;
        return 0;
    }

    std::ifstream code_file{argv[1]};
    std::vector<Instruction> program = parse(code_file);

    if (argc == 3) {
        std::string arg2{argv[2]};
        if(arg2 == "tobinary") {
            for(auto &inst: program) {
                std::cout << inst->to_binary() << std::endl;
            }
        }
    }

    Machine machine;
    machine.load(std::move(program));

RESUME:
    while (machine.next()) {
        
    }
        ;

    // Print register value
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i * 4 + j < 32) {
                std::cout << "$" << std::left << std::setw(2)
                          << std::setfill(' ') << i * 4 + j << ": ";
                try {
                    std::cout << std::bitset<32>{
                        static_cast<unsigned int>(machine.get_reg(i * 4 + j))};
                } catch (...) {
                    std::cout << "null                            ";
                }
                std::cout << "        ";
            }
        }
        std::cout << std::endl;
    }

    std::string str;
    std::cin >> str;

    if (str == "exit") {
        return 0;
    } else if (str == "next") {
        machine.next();
    } else if (str == "previous") {
        machine.pervious();
    } else if (str == "regfrom") {
        size_t reg;
        std::cin >> reg;
        if (dynamic_cast<word *>(machine.register_[reg].get())) {
            int from =
                dynamic_cast<word *>(machine.register_[reg].get())->from_;
            if (from != 0) {
                std::cout << "This register is from " << std::setw(20) << std::setfill('0') << from << std::setfill(' ') << std::endl;
            } else {
                std::cout << "This register is from nowhere" << std::endl;
            }
        } else {
            std::cout << "This register is from nowhere" << std::endl;
        }
    }


    goto RESUME;

    return 0;
}