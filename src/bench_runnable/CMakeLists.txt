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

    set(benchmark_directory ${source}_${type}_${number})
    set(name ${algo}_${source}_${type}_${number})
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
    set_target_properties(${name} PROPERTIES
                          RUNTIME_OUTPUT_DIRECTORY ${benchmark_directory}
                          OUTPUT_NAME ${algo})
endfunction()

# Code alignment ####################
# Stopped reproducing for some reason.
add_benchmark(code_alignment A int 1 USE_ARG)
add_benchmark(code_alignment A int 1)
add_benchmark(code_alignment_almost NO_ALGORITH int 0)

# Copy with reverse iterators
add_benchmark(copy_reverse_iterators std_copy int 1000)
add_benchmark(copy_reverse_iterators algo_copy int 1000)

# Lower bound ####################
function(add_lower_bound_benchmarks name type size)
  foreach(lb  algo_lower_bound
              algo_lower_bound_biased
              algo_lower_bound_biased_expensive_cmp
              algo_lower_bound_linear
              std_lower_bound)
    add_benchmark(${name} ${lb} ${type} ${size})
  endforeach()
endfunction()

add_lower_bound_benchmarks(lower_bound int 1000)
add_lower_bound_benchmarks(lower_bound double 1000)
add_lower_bound_benchmarks(lower_bound std_int64_t 1000)

add_lower_bound_benchmarks(lower_bound_first_5_percent int 1000)
add_lower_bound_benchmarks(lower_bound_first_5_percent double 1000)
add_lower_bound_benchmarks(lower_bound_first_5_percent std_int64_t 1000)

# Merge ###############################
function(add_merge_benchmarks name type size)
  foreach(merge  algo_merge
                 algo_merge_expensive_cmp
                 algo_merge_biased_first
                 algo_merge_biased_second
                 std_merge)
    add_benchmark(${name} ${merge} ${type} ${size})
  endforeach()
endfunction()

add_merge_benchmarks(merge int 2000)
add_merge_benchmarks(merge double 2000)
add_merge_benchmarks(merge std_int64_t 2000)


# Sort #########################
function(add_sort_benchmarks name type size)
  foreach(srt algo_stable_sort_sufficient_allocation
              std_sort
              std_stable_sort)
    add_benchmark(${name} ${srt} ${type} ${size})
  endforeach()
endfunction()

add_sort_benchmarks(sort int 1000)
add_sort_benchmarks(sort double 1000)
add_sort_benchmarks(sort std_int64_t 1000)

add_sort_benchmarks(sort int 10000)
add_sort_benchmarks(sort double 10000)
