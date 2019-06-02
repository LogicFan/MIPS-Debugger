#pragma once
#include <iostream>
#include <memory>
#include <vector>

class Data_base;
class Instruction_base;
class Machine;

using Data = std::unique_ptr<Data_base>;
using Instruction = std::unique_ptr<Instruction_base>;

class Data_base {
  public:
    Data_base() = default;
    Data_base(Data_base const &rhs) = default;
    Data_base(Data_base &&rhs) = default;
    virtual ~Data_base() = default;

    Data_base &operator=(Data_base const &rhs) = default;
    Data_base &operator=(Data_base &&rhs) = default;

    virtual Data clone() = 0;

    virtual std::ostream &print(std::ostream &out) = 0;
};

inline std::ostream &operator<<(std::ostream &out, Data const &data) {
    return data->print(out);
}