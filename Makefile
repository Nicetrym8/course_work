SHELL:=bash

build:
	clang++ -std=c++20 -o main.out -Wall  main.cpp types.cpp autodb.cpp 
clean:
	rm main.o