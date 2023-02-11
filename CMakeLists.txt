cmake_minimum_required(VERSION 3.22)

project(Graphxx
        DESCRIPTION "C++ graph library"
        HOMEPAGE_URL "https://github.com/Kaskeeeee/graphxx"
        LANGUAGES CXX
)

message("")
message("  _____                 _                ")
message(" / ____|               | |               ")
message("| |  __ _ __ __ _ _ __ | |__ __  ____  __")
message("| | |_ | '__/ _` | '_ \\| '_ \\\\ \\/ /\\ \\/ /")
message("| |__| | | | (_| | |_) | | | |>  <  >  < ")
message(" \\_____|_|  \\__,_| .__/|_| |_/_/\\_\\/_/\\_\\")
message("                 | |                     ")
message("                 |_|                     ")
message("")

# ###############################################################################
# CONFIGURATION OPTION
# ###############################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generates a file containing the exact compiler calls for all translation units 
# of the project in machine (needed for clang-tidy)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile with sanitizer checking
option(SANITIZE "Enable sanitizer option" OFF)

# Compile with coverage support
option(COVERAGE "Enable coverage output" OFF)

# Compile with benchmark
option(BUILD_BENCHMARK "Enable benchmarking" OFF)

# Compile with examples
option(BUILD_EXAMPLES "Enable examples" OFF)

# Compile with tests
option(BUILD_TEST "Enable tests" OFF)

# ###############################################################################
# COMPILER FLAGS
# ###############################################################################

include(CheckCXXCompilerFlag)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options("-Weverything")
    add_compile_options("-Wno-c++98-compat")
    add_compile_options("-Wno-c++98-compat-pedantic")
    add_compile_options("-Wno-padded")
    add_compile_options("-Wno-undefined-func-template")
    add_compile_options("-Wno-documentation-unknown-command")
    add_compile_options("-Wno-global-constructors")
    add_compile_options("-Wno-exit-time-destructors")
    add_compile_options("-Wno-sign-conversion")
    add_compile_options("-Wno-weak-vtables")
    add_compile_options("-Wno-weak-template-vtables")
    add_compile_options("-Wno-documentation")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wpedantic")
endif()

# ###############################################################################
# BUILD TYPE
# ###############################################################################

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options("-O3")
    add_compile_options("-DNDEBUG")

elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options("-ggdb3")
    elseif (CMAKE_CXX_COMPILER_ID EQUAL "Clang")
        add_compile_options("-ggdb")
    else()
        add_compile_options("-g")
    endif()
endif()

# ###############################################################################
# SANITIZE
# ###############################################################################

if(SANITIZE)
        add_compile_options("-O1")
        add_compile_options("-g")
        add_compile_options("-fno-omit-frame-pointer")
        add_compile_options("-fno-optimize-sibling-calls")

        add_compile_options("-fsanitize=address")
        add_compile_options("-fsanitize=leak")
        add_compile_options("-fsanitize=undefined")

        if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
            add_compile_options("-fsanitize=memory")
            add_compile_options("-fsanitize=integer")
            add_compile_options("-fsanitize=nullability")
        endif()

        set(LINKER_FLAGS ${LINKER_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer)
endif()


# ###############################################################################
# COVERAGE
# ###############################################################################

if(COVERAGE)
        add_compile_options("--coverage")
        set(LINKER_FLAGS ${LINKER_FLAGS} --coverage)

        # GET GCC MAJOR VERSION
        string(REGEX REPLACE "\\..*" "" GCC_MAJOR ${CMAKE_CXX_COMPILER_VERSION})

        add_custom_target(coverage
                COMMAND gcovr
                --gcov-executable gcov-${GCC_MAJOR}
                --root ${PROJECT_SOURCE_DIR}
                -e ${PROJECT_SOURCE_DIR}/third_party/catch2/catch.hpp
                -e ${PROJECT_SOURCE_DIR}/test/.*
                --html --html-details coverage.html
                COMMENT "Generating Code Coverage"
        )
endif()

# ###############################################################################
# LIBRARY
# ###############################################################################
add_library(graphxx OBJECT)

target_include_directories(graphxx
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms
        PUBLIC ${PROJECT_SOURCE_DIR}/include/io
        PUBLIC ${PROJECT_SOURCE_DIR}/include/generators
        PUBLIC ${PROJECT_SOURCE_DIR}/include/utils
        PUBLIC ${PROJECT_SOURCE_DIR}/third_party/pugixml
        PUBLIC ${PROJECT_SOURCE_DIR}/src
        PUBLIC ${PROJECT_SOURCE_DIR}/src/algorithms
        PUBLIC ${PROJECT_SOURCE_DIR}/src/io
        PUBLIC ${PROJECT_SOURCE_DIR}/src/generators
        PUBLIC ${PROJECT_SOURCE_DIR}/src/utils
)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.i.hpp)

target_sources(graphxx PRIVATE ${SRCS})

target_compile_options(graphxx
        PRIVATE ${COMPILER_FLAGS}
)

target_link_options(graphxx
        PRIVATE ${LINKER_FLAGS}
)

# ###############################################################################
# LINTER
# ###############################################################################

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    add_custom_target(tidy
                      COMMAND ${CLANG_TIDY_EXE} -config-file=${PROJECT_SOURCE_DIR}/.clang-tidy
                      ${PROJECT_SOURCE_DIR}/include/**/*.hpp
    )
endif()

# ###############################################################################
# DOCUMENTATION
# ###############################################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
        set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
        set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})
        set(DOXYGEN_HAVE_DOT NO)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_LATEX NO)
        set(DOXYGEN_GENERATE_MAN NO)
        set(DOXYGEN_GENERATE_RTF NO)
        set(DOXYGEN_GENERATE_XML NO)
        set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc)

        # Add a target for generating documentation with Doxygen
        doxygen_add_docs(doc
                ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src/algorithms ${PROJECT_SOURCE_DIR}/include/algorithms
                COMMENT "Generating API documentation with Doxygen"
        )
endif()

# ###############################################################################
# STATISTICS
# ###############################################################################

find_program(CLOC_FOUND cloc)
if (CLOC_FOUND)
    add_custom_target(stats
                      COMMAND cloc ${PROJECT_SOURCE_DIR}/src
                                   ${PROJECT_SOURCE_DIR}/include
                                   ${PROJECT_SOURCE_DIR}/test
    )
endif()

# ###############################################################################
# PREPARING TEST
# ###############################################################################

if (BUILD_TEST)
    add_executable(graphxx_test)

    target_sources(graphxx_test
        PRIVATE ${PROJECT_SOURCE_DIR}/test/test_main.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/utils_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/list_graph_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/matrix_graph_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/a_star_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/bellman_ford_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/bfs_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/dfs_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/dijkstra_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/floyd_warshall_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/ford_fulkerson_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/johnson_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/kruskal_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/tarjan_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graphml_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graphviz_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graph_generator_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graphml_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graphviz_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/third_party/catch2/catch.hpp
    )

    target_include_directories(graphxx_test SYSTEM
        PUBLIC ${PROJECT_SOURCE_DIR}/third_party/catch2
    )

    target_link_libraries(graphxx_test
        PUBLIC graphxx
    )

    target_compile_options(graphxx_test
        PRIVATE -Wno-unused-but-set-variable
        PRIVATE ${COMPILER_FLAGS}
    )

    target_link_options(graphxx_test
            PRIVATE ${LINKER_FLAGS}
    )

    install(
            TARGETS graphxx_test
            RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
    )

endif()


# ###############################################################################
# TESTS
# ###############################################################################

if (BUILD_TEST)

    enable_testing()

    add_test(NAME unit_tests
            COMMAND graphxx_test
    )

    find_program(VALGRIND_FOUND valgrind)
    if (VALGRIND_FOUND)
    add_test(NAME valgrindTest
            COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
            --track-origins=yes
            ${CMAKE_BINARY_DIR}/graphxx_test
    )
    endif()

endif()


# ###############################################################################
# EXAMPLES
# ###############################################################################

if (BUILD_EXAMPLES)
    add_executable(graphviz_example)
    target_sources(graphviz_example
            PRIVATE ${PROJECT_SOURCE_DIR}/examples/graphviz_example.cpp
    )

    target_link_libraries(graphviz_example
            PUBLIC graph
    )

    install(
            TARGETS graphviz_example
            RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
    )

endif()

# ###############################################################################
# BENCHMARK
# ###############################################################################

if(BUILD_BENCHMARK)
    # Fetch nanobench for benchmarking
    include(FetchContent)
    FetchContent_Declare(
            nanobench
            GIT_REPOSITORY https://github.com/martinus/nanobench.git
            GIT_TAG v4.1.0
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(nanobench)

    # Boost flags
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)

    # Find Boost
    find_package(Boost 1.71.0 COMPONENTS graph)

    if(Boost_FOUND)
            include_directories(${Boost_INCLUDE_DIRS})

            # dijkstra
            add_executable(benchmark_dijkstra)

            target_sources(benchmark_dijkstra
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_dijkstra.cpp
            )

            target_link_libraries(benchmark_dijkstra
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # kruskal
            add_executable(benchmark_kruskal)

            target_sources(benchmark_kruskal
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_kruskal.cpp
            )

            target_link_libraries(benchmark_kruskal
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # bfs
            add_executable(benchmark_bfs)

            target_sources(benchmark_bfs
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_bfs.cpp
            )

            target_link_libraries(benchmark_bfs
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # dfs
            add_executable(benchmark_dfs)

            target_sources(benchmark_dfs
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_dfs.cpp
            )

            target_link_libraries(benchmark_dfs
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # bellman_ford
            add_executable(benchmark_bellman_ford)

            target_sources(benchmark_bellman_ford
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_bellman_ford.cpp
            )

            target_link_libraries(benchmark_bellman_ford
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # floyd_warshall
            add_executable(benchmark_floyd_warshall)

            target_sources(benchmark_floyd_warshall
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_floyd_warshall.cpp
            )

            target_link_libraries(benchmark_floyd_warshall
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            # johnson
            add_executable(benchmark_johnson)

            target_sources(benchmark_johnson
                    PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/benchmark_johnson.cpp
            )

            target_link_libraries(benchmark_johnson
                    PUBLIC graphxx
                    PUBLIC ${Boost_LIBRARIES}
                    PRIVATE nanobench
            )

            install(
                    TARGETS benchmark_dijkstra benchmark_kruskal benchmark_bfs benchmark_dfs benchmark_bellman_ford benchmark_floyd_warshall benchmark_johnson
                    RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
            )
    endif()
endif()

# ###############################################################################
# CLEAN
# ###############################################################################

add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning build..."
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/build/*
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning doc..."
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/doc/*
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning bin..."
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/bin/*
        COMMAND ${CMAKE_COMMAND} -E echo "Done!"
)


# COMMANDS FOR BUILDING THE PROJECT:
# mkdir build
# cd build
# cmake -D<variable to modify>=<value> ..
# cmake --build .

# OTHER COMMANDS:
# <generator> test              execute tests
# <generator> install           install executables
# <generator> clean-all         delete generated files
# <generator> coverage          generate code coverage
# <generator> doc              generate documentation
# <generator> tidy             run linter
# <generator> stats            run cloc