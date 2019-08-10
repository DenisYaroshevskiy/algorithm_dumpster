#include_directories(../)

add_executable(tests)
target_sources(tests PRIVATE
               algorithm/find_nth.t.cc
               algorithm/make_vector_of_iterators.t.cc
               algorithm/memoized_function.t.cc
               algorithm/nth_permutation.t.cc
               algorithm/to_factoriadic_representation.t.cc
               algorithm/type_functions.t.cc
               algorithm/unroll.t.cc
               bench_generic/input_generators.t.cpp
               catch_main.cc)
target_compile_options(tests PRIVATE
                       -Werror -Wall -Wextra -Wpedantic -O2
                       -fsanitize=address -fno-omit-frame-pointer
                       --std=c++17)
target_link_options(tests PRIVATE -fsanitize=address)
set_target_properties(tests PROPERTIES CXX_STANDARD 17)
