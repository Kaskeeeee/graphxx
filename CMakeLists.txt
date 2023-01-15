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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generates a file containing the exact compiler calls for all translation units of the project in machine (needed for clang-tidy)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# find clang-tidy
# find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# Undefined behavior checking at runtime
OPTION(UBSAN "Undefined behaviour sanitizer option" OFF)

# Compile with coverage support
option(COVERAGE "Enable coverage output" OFF)

# Compile with benchmark
option(BENCHMARK "Enable benchmarking" OFF)

set(COMPILER_FLAGS
        -Wall -Wextra -Wpedantic
        $<$<BOOL:${UBSAN}>:-fsanitize=undefined -fno-omit-frame-pointer>
        $<$<BOOL:${COVERAGE}>:--coverage>
)

set(LINKER_FLAGS
        $<$<BOOL:${UBSAN}>:-fsanitize=undefined -fno-omit-frame-pointer>
        $<$<BOOL:${COVERAGE}>:--coverage>
)

# ###############################################################################
# VARIABLES
# ###############################################################################

# include
set(INC
        ${PROJECT_SOURCE_DIR}/include/base.hpp
        ${PROJECT_SOURCE_DIR}/include/graph_concepts.hpp
        ${PROJECT_SOURCE_DIR}/include/list_graph.hpp
        ${PROJECT_SOURCE_DIR}/include/matrix_graph.hpp
        ${PROJECT_SOURCE_DIR}/include/exceptions.hpp
        ${PROJECT_SOURCE_DIR}/include/utils.hpp
)

# include/algorithms
set(INC_ALG
        ${PROJECT_SOURCE_DIR}/include/algorithms/algorithms_base.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/bfs.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/dfs.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/bellman_ford.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/tarjan.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/dijkstra.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/kruskal.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/a_star.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/johnson.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/ford_fulkerson.hpp
)

# include/utils
set(INC_UTILS
        ${PROJECT_SOURCE_DIR}/include/utils/graph_generator.hpp
        ${PROJECT_SOURCE_DIR}/include/utils/tuple_utils.hpp
)

# include/io
set(INC_IO
        ${PROJECT_SOURCE_DIR}/include/io/graphviz.hpp
)

# src
set(SRC
        ${PROJECT_SOURCE_DIR}/src/list_graph.i.hpp
        ${PROJECT_SOURCE_DIR}/src/matrix_graph.i.hpp
        ${PROJECT_SOURCE_DIR}/src/exceptions.cpp
)

# src/algorithms
set(SRC_ALG
        ${PROJECT_SOURCE_DIR}/src/algorithms/bfs.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/dfs.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/bellman_ford.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/tarjan.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/dijkstra.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/kruskal.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/a_star.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/johnson.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/ford_fulkerson.i.hpp
)

# src/io
set(SRC_IO
        ${PROJECT_SOURCE_DIR}/src/io/graphviz.i.hpp
)

# include/utils
set(SRC_UTILS
        ${PROJECT_SOURCE_DIR}/src/utils/graph_generator.i.hpp
)

# ###############################################################################
# LIBRARY
# ###############################################################################
add_library(graph OBJECT)

target_sources(graph
        PUBLIC ${INC}
        PUBLIC ${INC_ALG}
        PUBLIC ${INC_IO}
        PUBLIC ${INC_UTILS}
        PRIVATE ${SRC}
        PRIVATE ${SRC_ALG}
        PRIVATE ${SRC_IO}
        PRIVATE ${SRC_UTILS}
)

target_include_directories(graph
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms
        PUBLIC ${PROJECT_SOURCE_DIR}/include/io
        PUBLIC ${PROJECT_SOURCE_DIR}/include/utils
        PUBLIC ${PROJECT_SOURCE_DIR}/third_party/pugixml
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms
        PRIVATE ${PROJECT_SOURCE_DIR}/src/io
)

target_compile_options(graph
        PRIVATE ${COMPILER_FLAGS}
)

target_link_options(graph
        PRIVATE ${LINKER_FLAGS}
)

# set_target_properties(graph PROPERTIES LINKER_LANGUAGE CXX)

# ###############################################################################
# COVERAGE
# ###############################################################################
if(COVERAGE)
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
# TIDY
# ###############################################################################
# add_custom_target(tidy
# COMMAND ${CLANG_TIDY_EXE} -config-file=${PROJECT_SOURCE_DIR}/.clang-tidy -p=${CMAKE_BINARY_DIR}/compile_commands.json ${SRC_UTILS} #use -list-checks for reading checks used
# )

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
endif(DOXYGEN_FOUND)

# ###############################################################################
# PREPARING TEST
# ###############################################################################
add_executable(graph_test)

target_sources(graph_test
        PRIVATE ${PROJECT_SOURCE_DIR}/test/test_main.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/digraph_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/bellman_ford_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/bfs_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/dfs_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/dijkstra_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/floyd_warshall_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/ford_fulkerson_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/tarjan_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/johnson_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/kruskal_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/graph_generator_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/third_party/catch2/catch.hpp
)

target_include_directories(graph_test
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(graph_test SYSTEM
        PUBLIC ${PROJECT_SOURCE_DIR}/third_party/catch2
)

target_link_libraries(graph_test
        PUBLIC graph
)

target_compile_options(graph_test
        PRIVATE -Wno-unused-but-set-variable
        PRIVATE ${COMPILER_FLAGS}
)

target_link_options(graph_test
        PRIVATE ${LINKER_FLAGS}
)

install(
        TARGETS graph_test
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

# ###############################################################################
# TESTS
# ###############################################################################
enable_testing()

add_test(NAME digraphTest
        COMMAND graph_test
)

add_test(NAME valgrindTest
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
        --track-origins=yes
        ${CMAKE_BINARY_DIR}/graph_test
)

# ###############################################################################
# EXAMPLES
# ###############################################################################

# graphviz example
# add_executable(graphviz_example)
# target_sources(graphviz_example
# PRIVATE ${PROJECT_SOURCE_DIR}/examples/graphviz_example.cpp
# )
#
# target_include_directories(graphviz_example
# PRIVATE ${PROJECT_SOURCE_DIR}/src
# )
#
# target_link_libraries(graphviz_example
# PUBLIC graph
# )
#
# install(
# TARGETS graphviz_example
# RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
# )
if(BENCHMARK)
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
                        PRIVATE ${PROJECT_SOURCE_DIR}/examples/benchmark_dijkstra.cpp
                )

                target_include_directories(benchmark_dijkstra
                        PRIVATE ${PROJECT_SOURCE_DIR}/src
                )

                target_link_libraries(benchmark_dijkstra
                        PUBLIC graph
                        PUBLIC ${Boost_LIBRARIES}
                        PRIVATE nanobench
                )

                # kruskal
                # add_executable(benchmark_krsukal)
                #
                # target_sources(benchmark_krsukal
                # PRIVATE ${PROJECT_SOURCE_DIR}/examples/benchmark_kruskal.cpp
                # )
                #
                # target_include_directories(benchmark_krsukal
                # PRIVATE ${PROJECT_SOURCE_DIR}/src
                # )
                #
                # target_link_libraries(benchmark_krsukal
                # PUBLIC graph
                # PUBLIC ${Boost_LIBRARIES}
                # PRIVATE nanobench
                # )
                #
                # # bfs
                # add_executable(benchmark_bfs)
                #
                # target_sources(benchmark_bfs
                # PRIVATE ${PROJECT_SOURCE_DIR}/examples/benchmark_bfs.cpp
                # )
                #
                # target_include_directories(benchmark_bfs
                # PRIVATE ${PROJECT_SOURCE_DIR}/src
                # )
                #
                # target_link_libraries(benchmark_bfs
                # PUBLIC graph
                # PUBLIC ${Boost_LIBRARIES}
                # PRIVATE nanobench
                # )
                #
                # # dfs
                # add_executable(benchmark_dfs)
                #
                # target_sources(benchmark_dfs
                # PRIVATE ${PROJECT_SOURCE_DIR}/examples/benchmark_dfs.cpp
                # )
                #
                # target_include_directories(benchmark_dfs
                # PRIVATE ${PROJECT_SOURCE_DIR}/src
                # )
                #
                # target_link_libraries(benchmark_dfs
                # PUBLIC graph
                # PUBLIC ${Boost_LIBRARIES}
                # PRIVATE nanobench
                # )
                #
                # # bellman_ford
                # add_executable(benchmark_bellman_ford)
                #
                # target_sources(benchmark_bellman_ford
                # PRIVATE ${PROJECT_SOURCE_DIR}/examples/benchmark_bellman_ford.cpp
                # )
                #
                # target_include_directories(benchmark_bellman_ford
                # PRIVATE ${PROJECT_SOURCE_DIR}/src
                # )
                #
                # target_link_libraries(benchmark_bellman_ford
                # PUBLIC graph
                # PUBLIC ${Boost_LIBRARIES}
                # PRIVATE nanobench
                # )
                install(
                        TARGETS benchmark_dijkstra # benchmark_krsukal benchmark_bfs benchmark_dfs benchmark_bellman_ford
                        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
                )
        endif()
endif()

# ###############################################################################
# CLEAN
# ###############################################################################
add_custom_target(clean_all
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning doc..."
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/doc
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning bin..."
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E echo "Done!"
)

# COMMANDS FOR BUILDING THE PROJECT:
# mkdir build
# cd build
# cmake -D<variable to modify> ..
# cmake --build .

# OTHER COMMANDS:
# <generator> test              execute tests
# <generator> install           install executables
# <generator> clean_all         delete /doc and /bin
# <generator> coverage          generate code coverage
# <genereator> doc              generate documentation
