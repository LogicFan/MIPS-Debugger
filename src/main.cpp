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
void run(Machine &machine);
void run(std::vector<Instruction> program);
void run(std::vector<Instruction> program, std::vector<int> const &array);
void run(std::vector<Instruction> program, int reg1, int reg2);
void symbol();
void binary(std::vector<Instruction> program);

extern std::map<std::string, size_t> symbol_table;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Version 0.3 (Beta)" << std::endl;
        return 0;
    }

    std::ifstream code_file{argv[1]};
    std::vector<Instruction> program = parse(code_file);

    if (argc == 2) {
        // be default, it is -run
        run(std::move(program));
    } else {
        std::string option{argv[2]};
        if (option == "-run") {
            run(std::move(program));
        } else if (option == "-number") {
            int reg1, reg2;

            std::cout << "Please input register 1 value:";
            std::cin >> reg1;
            std::cout << "Please input register 2 value:";
            std::cin >> reg2;

            run(std::move(program), reg1, reg2);
        } else if (option == "-array") {
            int size;
            std::vector<int> array;

            std::cout << "Please input the size of array:";
            std::cin >> size;
            for (int i = 0; i < size; ++i) {
                int num;
                std::cout << "Please input the " << i
                          << "-th element of array:";
                std::cin >> num;
                array.push_back(num);
            }

            run(std::move(program), array);
        } else if (option == "-symbol") {
            symbol();
        } else if (option == "-binary") {
            binary(std::move(program));
        } else {
            std::cout << "Unsupported option." << std::endl;
        }
    }

    return 0;
}

void binary(std::vector<Instruction> program) {
    for (auto const &inst : program) {
        std::cout << inst->to_binary(false);
    }
}

void symbol() {
    for (auto const &e : symbol_table) {
        std::cout << std::setw(20) << e.first << ": " << e.second << std::endl;
    }
}

void run(std::vector<Instruction> program) {
    Machine machine;
    machine.load(std::move(program));
    run(machine);
}

void run(std::vector<Instruction> program, std::vector<int> const &array) {
    Machine machine;
    machine.load(std::move(program));

    int A = machine.code_area_ + 1;
    for (int i = 0; i < array.size(); ++i) {
        machine.set_mem(A + i, array[i]);
    }
    machine.set_reg(1, A);
    machine.set_reg(2, array.size());
    run(machine);
}

void run(std::vector<Instruction> program, int reg1, int reg2) {
    Machine machine;
    machine.load(std::move(program));
    machine.set_reg(1, reg1);
    machine.set_reg(2, reg2);
    run(machine);
}

void run(Machine &machine) {
RESUME:

    try {
        while (machine.next()) {
        };
    } catch (std::runtime_error const &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << machine << std::endl;

    while (true) {
        std::string str;
        std::cin >> str;

        if (str == "exit") {
            exit(0);
        } else if (str == "next") {
            try {
                machine.next(true);
            } catch (std::runtime_error const &e) {
                std::cout << e.what() << std::endl;
            }
            std::cout << machine << std::endl;
        } else if (str == "previous") {
            machine.pervious();
            std::cout << machine << std::endl;
        } else if (str == "resume") {
            try {
                machine.next(true);
                goto RESUME;
            } catch (std::runtime_error const &e) {
                std::cout << e.what() << std::endl;
                std::cout << machine << std::endl;
            }
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
            size_t rollback_id;
            std::cin >> rollback_id;
            if (machine.roll_back(rollback_id)) {
                std::cout << machine << std::endl;
            }
        }
    }
}

void word_info(Instruction_base *reg) {
    if (reg) {
        std::cout << std::bitset<32>{
            static_cast<unsigned int>(reg->to_binary(false))};
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
