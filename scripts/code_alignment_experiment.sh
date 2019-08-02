out="code_alignment_experiment_res.txt"

echo "measuring alignment" > $out

for i in {1..1000}
do
  echo "i = " $i >> $out
  clang++ -lbenchmark --std=c++17 -O3 -DNOOP_COUNT=$i -I ../src ../src/code_alignment.cc ../src/google_benchmark_main.cc
  ./a.out >> $out
done