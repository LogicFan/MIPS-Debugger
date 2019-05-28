#include "Data_base.hpp"
#include <fstream>

int main() {
    std::ifstream file{"./test.mips"};
    std::string s;
    while(getline(file, s)) {
        std::cout << Data_base::parse(s) << std::endl;
    }
}