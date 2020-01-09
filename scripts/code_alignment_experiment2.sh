 # Copyright 2019 Denis Yaroshevskiy
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

out="code_alignment_experiment_res.txt"

echo "measuring alignment" > $out

for i in {0..128}
do
  echo "i = " $i >> $out
  clang++ -lbenchmark --std=c++17 -O3 -DNOOP_COUNT=$i -I ../src ../src/bench_runnable/code_alignment_experiment2.cc ../src/bench_runnable/google_benchmark_main.cc
  ./a.out >> $out
done
