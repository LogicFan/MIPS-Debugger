main: ./src/main.cpp
	g++ -std=c++17 ./src/main.cpp ./src/Machine.cpp \
	./src/Data/Instructions/Instruction_base.cpp ./src/Parser/Parser.cpp \
	-o ./bin/mips-debugger -g

clean:
	rm -rf ./build
	rm -rf ./bin
