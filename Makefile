main: ./src/main.cpp
	mkdir bin
	g++ -std=c++17 ./src/main.cpp ./src/Machine.cpp ./src/Data/Data_base.cpp \
	./src/Data/Instructions/Instruction_base.cpp ./src/Parser/Parser.cpp \
	-o ./bin/mips-debugger -g

clean:
	rm -rf ./build
	rm -rf ./bin
