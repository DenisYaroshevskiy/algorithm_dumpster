#
# Copyright 2020 Denis Yaroshevskiy
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

find_package(benchmark)

function(add_benchmark name source)
    set(compiler_options --std=c++17 --stdlib=libc++ -g -Werror -Wall -Wextra -Wpedantic -O3 -march=native -mllvm -align-all-functions=7)
    add_executable(${name})
    target_sources(${name} PRIVATE ${source})
    target_compile_options(${name} PRIVATE ${compiler_options})
    target_link_libraries(${name} PUBLIC benchmark pthread)
    target_link_options(${name} PRIVATE -stdlib=libc++)
endfunction()

add_benchmark(lower_bound lower_bound.cc)

add_benchmark(std_remove std_remove.cc)
add_benchmark(unsq_remove unsq_remove.cc)
add_benchmark(unsq_reduce_v1 unsq_reduce_v1.cc)
add_benchmark(std_min_element std_min_element.cc)
add_benchmark(std_reduce std_reduce.cc)
