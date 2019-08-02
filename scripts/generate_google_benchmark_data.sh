#!/bin/sh -x
rm ../data/google_benchmark_461/*

clang++ -lbenchmark --std=c++17 -O3 -g -DUSE_ARG -I ../src ../src/code_alignment.cc ../src/google_benchmark_main.cc -o fast.out
./fast.out
clang++ --std=c++17 -O3 -S -DUSE_ARG  -I ../src ../src/code_alignment.cc -o ../data/google_benchmark_461/fast.s
objdump -disassemble fast.out > ../data/google_benchmark_461/fast_objdump.txt
objdump -disassemble -no-leading-addr -no-show-raw-insn -macho -source fast.out > ../data/google_benchmark_461/fast_clean_objdump.txt

clang++ -lbenchmark --std=c++17 -O3 -g -I ../src ../src/code_alignment.cc ../src/google_benchmark_main.cc -o slow.out
./slow.out
clang++ --std=c++17 -O3 -S -I ../src ../src/code_alignment.cc -o ../data/google_benchmark_461/slow.s
objdump -disassemble slow.out > ../data/google_benchmark_461/slow_objdump.txt
objdump -disassemble -no-leading-addr -no-show-raw-insn -macho -source  slow.out > ../data/google_benchmark_461/slow_clean_objdump.txt

diff ../data/google_benchmark_461/fast.s ../data/google_benchmark_461/slow.s > ../data/google_benchmark_461/asm_diff.diff
diff ../data/google_benchmark_461/fast_objdump.txt ../data/google_benchmark_461/slow_objdump.txt > ../data/google_benchmark_461/objdump_diff.diff
diff ../data/google_benchmark_461/fast_clean_objdump.txt ../data/google_benchmark_461/slow_clean_objdump.txt > ../data/google_benchmark_461/objdump_clean_diff.diff
