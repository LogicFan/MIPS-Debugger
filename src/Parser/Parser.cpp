#include "Parser.hpp"
#include "../Data/BreakPoint/BreakPoint.hpp"
#include "../Data/Data_base.hpp"
#include "../Data/Instructions/Instruction_base.hpp"
#include "../Data/Instructions/Instructions.hpp"
#include "../Data/Label/Label.hpp"
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>

std::map<std::string, size_t> symbol_table;
std::set<size_t> labeled_address;

int parseInt(std::string s) {
    unsigned int i;
    std::stringstream ss{s};
    ss >> i;
    return static_cast<int>(i);
}

int parseHex(std::string s) {
    unsigned int i;
    std::stringstream ss{s};
    ss >> std::hex >> i;
    return static_cast<int>(i);
}

Instruction scan(std::string line) {
    static bool break_point = false;
    static size_t file_line = 0;
    static size_t code_line = 0;

    Instruction inst;

    bool break_temp = break_point;

    // omit space
    size_t s = 0;
    while (line[s] == ' ') {
        s = s + 1;
    }
    line = line.substr(s);

    // find comment
    s = line.find(';');

    if (s != std::string::npos) {
        // There is a comment
        if (line.find("break", s) != std::string::npos) {
            // There is a break point
            break_point = true;
        }
        line = line.substr(0, s);
    }

    // find label
    s = line.find(':');
    while (s != std::string::npos) {
        std::string label = line.substr(0, s);
        std::regex label_regex{"[a-zA-Z][0-9a-zA-Z]*"};
        if (std::regex_match(label, label_regex)) {
            if (symbol_table.count(label) != 0) {
                std::cout << "Error: duplicate label at line " << file_line
                          << "!" << std::endl;
            } else {
                symbol_table[label] = code_line;
                labeled_address.insert(code_line);
            }
        } else {
            std::cout << "Error: wrong syntax at line " << file_line << "!"
                      << std::endl;
            exit(0);
        }

        // omit space
        s = s + 1;
        while (line[s] == ' ') {
            s = s + 1;
        }

        line = line.substr(s);
        s = line.find(':');
    }

    // find instruction
    if (line != "") {
        // there exists a instruction
        std::stringstream inst_stream{line};
        std::string inst_id;
        inst_stream >> inst_id;

        std::regex integer{"-?[0-9]+"};
        std::regex hex_int{"0x[0-9a-fA-F]*"};
        std::regex identifer{"[a-zA-Z][0-9a-zA-Z]*"};

        if (inst_id == ".word") {
            std::string literal;
            inst_stream >> literal;
            if (std::regex_match(literal, integer)) {
                inst = std::make_unique<word>(parseInt(literal), 0);
            } else if (std::regex_match(literal, hex_int)) {
                inst = std::make_unique<word>(parseHex(literal), 0);
            } else if (std::regex_match(literal, identifer)) {
                inst = std::make_unique<word>(literal);
            } else {
                std::cout << "Error: wrong syntax at line " << file_line << "!"
                          << std::endl;
                exit(0);
            }
        } else if (inst_id == "add") {
            unsigned int d, s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<ADD>(d, s, t);
        } else if (inst_id == "sub") {
            unsigned int d, s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<SUB>(d, s, t);
        } else if (inst_id == "mult") {
            unsigned int s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<MULT>(s, t);
        } else if (inst_id == "multu") {
            unsigned int s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<MULTU>(s, t);
        } else if (inst_id == "div") {
            unsigned int s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<DIV>(s, t);
        } else if (inst_id == "divu") {
            unsigned int s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<DIVU>(s, t);
        } else if (inst_id == "mfhi") {
            unsigned int d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst = std::make_unique<MFHI>(d);
        } else if (inst_id == "mflo") {
            unsigned int d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst = std::make_unique<MFLO>(d);
        } else if (inst_id == "lis") {
            unsigned int d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst = std::make_unique<LIS>(d);
        } else if (inst_id == "lw") {
            int s, t;
            short i;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               ',');
            inst_stream >> i;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst = std::make_unique<LW>(s, t, static_cast<unsigned short>(i));
        } else if (inst_id == "sw") {
            int s, t;
            short i;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               ',');
            inst_stream >> i;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst = std::make_unique<SW>(s, t, static_cast<unsigned short>(i));
        } else if (inst_id == "slt") {
            unsigned int d, s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<SLT>(d, s, t);
        } else if (inst_id == "sltu") {
            unsigned int d, s, t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> d;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst = std::make_unique<SLTU>(d, s, t);
        } else if (inst_id == "beq") {
            int s, t;
            std::string label;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               ',');
            inst_stream >> label;
            if (std::regex_match(label, integer)) {
                inst = std::make_unique<BEQ>(
                    s, t, static_cast<short>(parseInt(label)));
            } else if (std::regex_match(label, identifer)) {
                inst = std::make_unique<BEQ>(s, t, label);
            } else {
                std::cout << "Error: wrong syntax at line " << file_line << "!"
                          << std::endl;
                exit(0);
            }
        } else if (inst_id == "bne") {
            int s, t;
            std::string label;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> t;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               ',');
            inst_stream >> label;
            if (std::regex_match(label, integer)) {
                inst = std::make_unique<BNE>(
                    s, t, static_cast<short>(parseInt(label)));
            } else if (std::regex_match(label, identifer)) {
                inst = std::make_unique<BNE>(s, t, label);
            } else {
                std::cout << "Error: wrong syntax at line " << file_line << "!"
                          << std::endl;
                exit(0);
            }
        } else if (inst_id == "jr") {
            unsigned int s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst = std::make_unique<JR>(s);
        } else if (inst_id == "jalr") {
            unsigned int s;
            inst_stream.ignore(std::numeric_limits<std::streamsize>::max(),
                               '$');
            inst_stream >> s;
            inst = std::make_unique<JALR>(s);
            labeled_address.insert(code_line + 4);
        }

        code_line += 4;

        inst->set_break(break_temp);
        inst->set_file_line(file_line);
    }
    ++file_line;

    return std::move(inst);
}

std::vector<Instruction> parse(std::istream &in) {

    std::string text;
    std::vector<Instruction> instructions;

    while (getline(in, text)) {
        Instruction inst = scan(text);
        if(inst) {
            instructions.push_back(inst);
        }
    }

    size_t code_line = 0;
    for (auto &inst : instructions) {
        inst->resolve_symbol(code_line);
        code_line += 4;
    }

    return std::move(instructions);
}
