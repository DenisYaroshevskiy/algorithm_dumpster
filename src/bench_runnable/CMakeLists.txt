function(add_benchmark source macros)
    set(compiler_options --std=c++17 -g -Werror -Wall -Wextra -Wpedantic -O3 -march=native)

    set(name benchmark_${source})
    if (macros STREQUAL '')
    else()
      set(name ${name}_${macros})
      set(compiler_options ${compiler_options} -D${macros})
    endif()

    add_executable(${name})
    target_sources(${name} PRIVATE
                   ${source}.cc
                   google_benchmark_main.cc
                   )
    target_compile_options(${name} PRIVATE ${compiler_options})
    target_link_libraries(${name} benchmark)
endfunction()

# Code alignment
add_benchmark(code_alignment '')
add_benchmark(code_alignment USE_ARG)
add_benchmark(code_alignment_almost '')

# Sort
add_benchmark(sort STD_SORT)
add_benchmark(sort STD_STABLE_SORT)