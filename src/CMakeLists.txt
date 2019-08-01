if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

project(benchmarking_sets)

include_directories(./)
add_subdirectory(test)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

add_executable(benchmarks)
target_sources(benchmarks PRIVATE
               code_alignment.cc
               google_benchmark_main.cc)
set_target_properties(benchmarks PROPERTIES CXX_STANDARD 17)
target_compile_options(benchmarks PRIVATE -Werror -Wall -Wextra -Wpedantic -g -O3)
target_link_libraries(benchmarks benchmark)
