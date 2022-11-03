SHELL:=bash

build:
	clang++ -O3 -std=c++20 -o main.out -Wall  main.cpp types.cpp  autodb.cpp 
build_gen:
	clang++ -O3 -std=c++20 -o generator.out -Wall  generator.cpp types.cpp
test:
	valgrind ./main.out -t data.bin && time ./main.out -t data.bin
clean:
	rm main.o