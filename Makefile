SHELL:=bash
f:=20000
build:
	clang++ -O3 -std=c++20 -o main.out -Wall  main.cpp types.cpp  autodb.cpp 
build_gen:
	clang++ -O3 -std=c++20 -o generator.out -Wall  generator.cpp types.cpp
test_memory_leaks:
	export DEBUGINFOD_URLS="https://debuginfod.archlinux.org" && valgrind ./main.out -w $f test_data.bin
test_memory_usage:
	valgrind --tool=massif --pages-as-heap=yes --massif-out-file=massif.out ./main.out -w $f test_data.bin; grep mem_heap_B massif.out | sed -e 's/mem_heap_B=\(.*\)/\1/' | sort -g | tail -n 1; massif-visualizer massif.out
test_time_usage:
	time ./main.out -w $f test_data.bin
clean:
	rm main.out generator.out test_data.bin