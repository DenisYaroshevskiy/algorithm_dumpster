#include_directories(../)

add_executable(tests)
target_sources(tests PRIVATE
               algorithm/memoized_function.t.cc
               algorithm/unroll.t.cc
               catch_main.cc)
target_compile_options(tests PRIVATE
                       -Werror -Wall -Wextra -Wpedantic -O2
                       -fsanitize=address -fno-omit-frame-pointer
                       --std=c++17)
target_link_options(tests PRIVATE -fsanitize=address)
set_target_properties(tests PROPERTIES CXX_STANDARD 17)
