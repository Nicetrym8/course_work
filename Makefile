SHELL:=bash

build:
	clang++ -std=c++20 -o main.out -Wall  main.cpp types.cpp autodb.cpp 
memory_test:
	valgrind ./main.out -t data.bin
clean:
	rm main.o