cmake_minimum_required(VERSION 3.7)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_subdirectory(src)
