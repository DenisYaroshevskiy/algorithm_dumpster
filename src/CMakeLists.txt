if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

project(algorithm_dumpster)

include_directories(./)
add_subdirectory(test)
add_subdirectory(bench_runnable)
