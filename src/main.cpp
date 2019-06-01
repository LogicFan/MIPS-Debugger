#include "Data/BreakPoint/BreakPoint.hpp"
#include "Data/Data_base.hpp"
#include "Data/Instructions/Instruction_base.hpp"
#include "Data/Instructions/Instructions.hpp"
#include "Data/Label/Label.hpp"
#include "Machine.hpp"
#include "Parser/Parser.hpp"
#include <fstream>
#include <iostream>

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

    while (true) {
        std::cout << machine << std::endl;

        std::string str;
        std::cin >> str;

        if (str == "exit") {
            return 0;
        } else if (str == "next") {
            machine.next(true);
        } else if (str == "previous") {
            machine.pervious();
        } else if (str == "resume") {
            machine.next(true);
            goto RESUME;
        } else if (str == "regfrom") {
            size_t reg;
            std::cin >> reg;
            if (dynamic_cast<word *>(machine.register_[reg].get())) {
                int from =
                    dynamic_cast<word *>(machine.register_[reg].get())->from_;
                if (from != 0) {
                    std::cout << "This register is from " << std::right
                              << std::setw(20) << std::setfill('0') << from
                              << std::setfill(' ') << std::left << std::endl;
                } else {
                    std::cout << "This register is from nowhere" << std::endl;
                }
            } else {
                std::cout << "This register is from nowhere" << std::endl;
            }
        }
    }

    return 0;
}