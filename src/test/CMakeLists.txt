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

#include_directories(../)

add_executable(tests)

target_sources(tests PRIVATE
               algo/advance_up_to.t.cc
               algo/binary_search_biased.t.cc
               algo/binary_search.t.cc
               algo/comparisons.t.cc
               algo/container_cast.t.cc
               algo/copy.t.cc
               algo/factoriadic_representation.t.cc
               algo/factorial.t.cc
               algo/find_nth.t.cc
               algo/half_positive.t.cc
               algo/make_vector_of_iterators.t.cc
               algo/memoized_function.t.cc
               algo/merge_biased.t.cc
               algo/merge.t.cc
               algo/move.t.cc
               algo/nth_permutation.t.cc
               algo/type_functions.t.cc
               algo/unroll.t.cc
               bench_generic/input_generators.t.cpp
               catch_main.cc)
target_compile_options(tests PRIVATE
                       -Werror -Wall -Wextra -Wpedantic -O2 -g
                       -fsanitize=address -fno-omit-frame-pointer
                       --std=c++17)

target_link_options(tests PRIVATE -fsanitize=address)
set_target_properties(tests PROPERTIES CXX_STANDARD 17)
