#include "./Data/Instructions/Instructions.hpp"
#include "Machine.hpp"

#include <cassert>
#include <iostream>

#define assert_error(stmt)                                                     \
    try {                                                                      \
        stmt assert(false);                                                    \
    } catch (std::runtime_error const &e) {                                    \
    }

#define assert_warning(stmt)                                                   \
    std::cout << "print warning: ";                                            \
    stmt

void machine_test() {
    Machine m;

    assert_error(m.set_reg(32, 10););
    // [Error: Accessing invalid register!]

    assert_error(m.get_reg(32););
    // [Error: Accessing invalid register!]

    assert_error(m.get_reg(5););
    // [Error: Accessing non-initialized register!]

    assert_error(m.get_high(););
    // [Error: Accessing non-initialized register!]

    assert_error(m.get_low(););
    // [Error: Accessing non-initialized register!]

    assert_warning(m.set_reg(0, 30););

    for (int i = 1; i < 32; ++i) {
        m.set_reg(i, 30 + i);
    }

    assert(m.get_reg(0) == 0);

    for (int i = 1; i < 32; ++i) {
        assert(m.get_reg(i) == 30 + i);
    }

    m.set_high(15);
    m.set_low(20);
    assert_warning(m.set_pc(22););

    assert(m.get_high() == 15);
    assert(m.get_low() == 20);
    assert(m.get_pc() == 22);

    m.set_break();
    assert(m.break_ == true);

    m.counter_ = 10;

    auto undo = [](Machine &machine) { assert_warning(machine.set_pc(4);) };

    m.add_undo(undo);

    m.pervious();

    assert(m.get_pc() == 4);
    assert(m.counter_ == 9);

    assert_error(m.set_mem(2, 1););
    assert_error(m.get_mem(4););

    std::vector<Instruction> code;
    for (int i = 0; i < 20; ++i) {
        code.push_back(std::make_unique<word>(i, 0));
    }

    m.load(std::move(code));
    assert(m.code_area_ == 80);
    assert(m.get_mem(76) == 19);

    assert_warning(m.set_pc(76););

    assert_warning(m.set_mem(4, 7));

    // need to test
    // exec, next, start, roll_back
}

int main() {
    machine_test();
    std::cout << "machine_test pass." << std::endl;
}