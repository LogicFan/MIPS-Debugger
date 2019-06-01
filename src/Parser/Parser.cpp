#include "Parser.hpp"
#include "../Data/BreakPoint/BreakPoint.hpp"
#include "../Data/Data_base.hpp"
#include "../Data/Instructions/Instruction_base.hpp"
#include "../Data/Instructions/Instructions.hpp"
#include "../Data/Label/Label.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>

std::map<std::string, size_t> symbol_table;
std::set<size_t> labeled_address;

std::vector<Instruction> parse(std::istream &in) {
    int file_line{0};
    int code_line{0};
    std::string text;

    bool break_{false};

    std::vector<Instruction> instructions;

    while (getline(in, text)) {
        Data data = Data_base::parse(text);

        if (dynamic_cast<BreakPoint *>(data.get())) {
            // This is a break point
            break_ = true;
        } else if (dynamic_cast<Label *>(data.get())) {
            // This is a label
            Label *label = dynamic_cast<Label *>(data.get());
            labeled_address.insert(code_line);
            symbol_table.insert(
                std::pair<std::string, size_t>(label->get_label(), code_line));
        } else if (dynamic_cast<Instruction_base *>(data.get())) {
            // This is a instruction

            Instruction inst =
                Instruction{dynamic_cast<Instruction_base *>(data.release())};

            inst->set_break(break_);
            inst->set_file_line(file_line);

            instructions.push_back(std::move(inst));

            break_ = false;
            code_line += 4;
            // instruction after JALR is also a labeled_address
            if (dynamic_cast<JALR *>(data.get())) {
                labeled_address.insert(code_line);
            }
        }
        ++file_line;
    }

    code_line = 0;
    for (auto &inst : instructions) {
        inst->resolve_symbol(code_line);
        code_line += 4;
    }

    return std::move(instructions);
}