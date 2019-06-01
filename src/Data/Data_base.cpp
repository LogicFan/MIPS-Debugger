#include "./Data_base.hpp"
#include "./BreakPoint/BreakPoint.hpp"
#include "./Instructions/Instructions.hpp"
#include "./Label/Label.hpp"
#include <sstream>

Data Data_base::parse(std::string const &text) {
    if (text == "; break") {
        return std::make_unique<BreakPoint>();
    } else if (text[text.size() - 1] == ':') {
        return std::make_unique<Label>(text.substr(0, text.size() - 1));
    } else {
        std::stringstream ss{text};
        std::string inst;
        ss >> inst;
        if (inst == ".word") {
            std::string literal;
            ss >> literal;
            if (('0' <= literal[0] && literal[0] <= '9') || literal[0] == '-') {
                short i;
                std::stringstream parse_int{literal};
                parse_int >> i;
                return std::make_unique<word>(i, 0);
            } else {
                return std::make_unique<word>(literal);
            }
        } else if (inst == "add") {
            unsigned int d, s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<ADD>(d, s, t);
        } else if (inst == "sub") {
            unsigned int d, s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<SUB>(d, s, t);
        } else if (inst == "mult") {
            unsigned int s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<MULT>(s, t);
        } else if (inst == "multu") {
            unsigned int s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<MULTU>(s, t);
        } else if (inst == "div") {
            unsigned int s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<DIV>(s, t);
        } else if (inst == "divu") {
            unsigned int s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<DIVU>(s, t);
        } else if (inst == "mfhi") {
            unsigned int d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            return std::make_unique<MFHI>(d);
        } else if (inst == "mflo") {
            unsigned int d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            return std::make_unique<MFLO>(d);
        } else if (inst == "lis") {
            unsigned int d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            return std::make_unique<LIS>(d);
        } else if (inst == "lw") {
            int s, t;
            short i;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            ss >> i;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            return std::make_unique<LW>(s, t, static_cast<unsigned short>(i));
        } else if (inst == "sw") {
            int s, t;
            short i;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            ss >> i;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            return std::make_unique<SW>(s, t, static_cast<unsigned short>(i));
        } else if (inst == "slt") {
            unsigned int d, s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<SLT>(d, s, t);
        } else if (inst == "sltu") {
            unsigned int d, s, t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> d;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            return std::make_unique<SLTU>(d, s, t);
        } else if (inst == "beq") {
            int s, t;
            std::string label;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            ss >> label;
            if (('0' <= label[0] && label[0] <= '9') || label[0] == '-') {
                short i;
                std::stringstream parse_int{label};
                parse_int >> i;
                return std::make_unique<BEQ>(s, t, static_cast<unsigned short>(i));
            } else {
                return std::make_unique<BEQ>(s, t, label);
            }
        } else if (inst == "bne") {
            int s, t;
            std::string label;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> t;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), ',');
            ss >> label;
            if (('0' <= label[0] && label[0] <= '9') || label[0] == '-') {
                short i;
                std::stringstream parse_int{label};
                parse_int >> i;
                return std::make_unique<BNE>(s, t, static_cast<unsigned short>(i));
            } else {
                return std::make_unique<BNE>(s, t, label);
            }
        } else if (inst == "jr") {
            unsigned int s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            return std::make_unique<JR>(s);
        } else if (inst == "jalr") {
            unsigned int s;
            ss.ignore(std::numeric_limits<std::streamsize>::max(), '$');
            ss >> s;
            return std::make_unique<JALR>(s);
        }
    }

    return std::unique_ptr<Data_base>{nullptr};
}