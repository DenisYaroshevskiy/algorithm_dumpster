#
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

add_executable(tests)

target_sources(tests PRIVATE
               algo/advance_up_to.t.cc
               algo/apply_rearrangment.t.cc
               algo/binary_counter.t.cc
               algo/binary_search_biased.t.cc
               algo/binary_search.t.cc
               algo/comparisons.t.cc
               algo/container_cast.t.cc
               algo/copy.t.cc
               algo/factoriadic_representation.t.cc
               algo/factorial.t.cc
               algo/find_nth.t.cc
               algo/half_nonnegative.t.cc
               algo/memoized_function.t.cc
               algo/merge_biased.t.cc
               algo/merge.t.cc
               algo/mersenne_primes.t.cc
               algo/move.t.cc
               algo/nth_permutation.t.cc
               algo/positions.t.cc
               algo/quadratic_sort.t.cc
               algo/shuffle_biased.t.cc
               algo/stable_sort.t.cc
               algo/strcmp.t.cc
               algo/strlen.t.cc
               algo/type_functions.t.cc
               algo/uint_tuple.t.cc
               algo/unroll.t.cc
               bench_generic/counting_benchmark.t.cc
               bench_generic/input_generators.t.cc
               simd/bits.t.cc
               simd/mm.t.cc
               simd/pack.t.cc
               unsq/drill_down.t.cc
               unsq/find.t.cc
               unsq/remove.t.cc
               unsq/unsq.t.cc
               catch_main.cc)
target_compile_options(tests PRIVATE
                       -Werror -Wall -Wextra -Wpedantic -Og -g
                       -fsanitize=address -fno-omit-frame-pointer
                       --std=c++17
                       -stdlib=libc++
                       -march=native)

target_link_options(tests PRIVATE -fsanitize=address -stdlib=libc++)
set_target_properties(tests PROPERTIES CXX_STANDARD 17)
