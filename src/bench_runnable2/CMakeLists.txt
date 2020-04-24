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
set(compiler_options --std=c++17 --stdlib=libc++ -g -Werror -Wall -Wextra -Wpedantic -O3 -march=native -mllvm -align-all-functions=7)
add_executable(remove)
target_sources(remove PRIVATE
               remove.cc)
target_compile_options(remove PRIVATE ${compiler_options})
target_link_libraries(remove PUBLIC benchmark pthread)
target_link_options(remove PRIVATE -stdlib=libc++)
