# MIPS-Debugger

This debugger is design for debugging MIPS program. It supports break point, stepper and gives warning and error when the program encounter problems. It also support print symbol table and output binary files.

## Getting started

To run this program, you shall type 

```
$ mips-debugger [input file] [option]
```

> Input file is MIPS assembly code.

> There are 5 options, `-binary` and `-symbol`, `run`, `-array`, `-number`. `-binary` will print the binary code; `-symbol` will print the symbol table; `-run` will directly run the program; `-array` will run the program with an array; `-number` will run the program with two number on register 1 and register 2. If there is no option provided, by default, it is `-run`.

## Break point

A break point is 

```
; break
```

and program will pause at the break point, print all registers and enter debug command mode.

## Warnings & Errors

When the MIPS program has some dangerous behavior, the debugger will print warning. Currently support the following warnings:

1. Warning: Arithmetic operation on non-word instruction!
2. Warning: Executing word instruction!
3. Warning: Modifying code area!
4. Warning: Modifying register 0!
5. Warning: Jumping to non-code area!
6. Warning: Jumping to non-label address!

When the MIPS program encounter an error, the debugger will print the error, pause the program, print all registers and enter debug command mode. Currently support the following errors:

1. Error: Accessing invalid register!
2. Error: Accessing non-aligned memory address!
3. Error: Accessing non-initialized memory!
4. Error: Accessing non-initialized register!
5. Error: Dividing zero!
6. Error: Executing invalid instruction!

## Debug command mode 

When the debugger enter debug command mode, there are some commands that can help us debug the program.

- `next`: The program will go one step further.
- `previous`: The program will go back one step.
- `resume`: The program will continue to run until next break point or error.
- `register [reg]`: Print the value of register `[reg]`, and also print the execution id corresponding to assignment this value to this register. `[reg]` can be `0` to `31`, `pc`, `hi`, `lo`.
- `memory [mem]`: Print the value at memory location `[mem]`, and also print execution id corresponding to assignment this value. `[mem]` can be any number divisible by 4.
- `rollback [exec_id]`: This will roll back to the state after the instruction corresponding with `[exec_id]` is executed.

## License

This software is under GNU Affero General Public License v3.0.