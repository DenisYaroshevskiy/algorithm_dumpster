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

function(add_benchmark source algo type number)
    set(compiler_options --std=c++17 -g -Werror -Wall -Wextra -Wpedantic -O3 -march=native)

    set(name benchmark_${source}_${algo}_${type}_${number})
    set(compiler_options ${compiler_options} -DSELECTED_ALGORITHM=${algo} -DSELECTED_TYPE=${type} -DSELECTED_NUMBER=${number})

    foreach(extra ${ARGN})
      set(name ${name}_${extra})
      set(compiler_options ${compiler_options} -D${extra})
    endforeach()

    add_executable(${name})
    target_sources(${name} PRIVATE
                   ${source}.cc
                   google_benchmark_main.cc
                   )
    target_compile_options(${name} PRIVATE ${compiler_options})
    target_link_libraries(${name} benchmark)
endfunction()

# Code alignment
# Stopped reproducing for some reason.
add_benchmark(code_alignment A int 1 USE_ARG)
add_benchmark(code_alignment A int 1)
add_benchmark(code_alignment_almost NO_ALGORITH int 0)

# Sort
add_benchmark(sort std_sort int 1000)
add_benchmark(sort std_stable_sort int 1000)

# Copy with reverse iterators
add_benchmark(copy_reverse_iterators std_copy int 1000)
add_benchmark(copy_reverse_iterators algo_copy int 1000)

# Lower bound
add_benchmark(lower_bound std_lower_bound int 1000)
add_benchmark(lower_bound algo_lower_bound int 1000)
