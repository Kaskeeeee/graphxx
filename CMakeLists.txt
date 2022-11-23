cmake_minimum_required(VERSION 3.22)

project(GraphLib
        DESCRIPTION "Graph library"
        HOMEPAGE_URL "https://github.com/Kaskeeeee/graph"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generates a file containing the exact compiler calls for all translation units of the project in machine (needed for clang-tidy)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Undefined behavior checking at runtime
OPTION(UBSAN "Undefined behaviour sanitizer option" OFF)

# Compile with coverage support
option(COVERAGE "Enable coverage output" OFF)

set(COMPILER_FLAGS
        -Wall -Wextra -Wpedantic
        $<$<BOOL:${UBSAN}>:-fsanitize=undefined -fno-omit-frame-pointer>
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
        ${PROJECT_SOURCE_DIR}/include/id_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/exceptions.hpp
        ${PROJECT_SOURCE_DIR}/include/utils.hpp
)

# include/algorithms
set(INC_ALG
        ${PROJECT_SOURCE_DIR}/include/algorithms/algorithms_base.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/bfs.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/dfs.hpp
        ${PROJECT_SOURCE_DIR}/include/algorithms/bellman_ford.hpp
)

# src
set(SRC
        ${PROJECT_SOURCE_DIR}/src/id_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/list_graph.i.hpp
        ${PROJECT_SOURCE_DIR}/src/matrix_graph.i.hpp
        ${PROJECT_SOURCE_DIR}/src/exceptions.cpp
)

# src/algorithms
set(SRC_ALG
        ${PROJECT_SOURCE_DIR}/src/algorithms/bfs.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/dfs.i.hpp
        ${PROJECT_SOURCE_DIR}/src/algorithms/bellman_ford.i.hpp
)

# ###############################################################################
# LIBRARY
# ###############################################################################
add_library(graph OBJECT)

# target_sources(graph
# PUBLIC ${PROJECT_SOURCE_DIR}/include/base.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/graph_concepts.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/list_graph.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/matrix_graph.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/id_manager.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/algorithms_base.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/bfs.hpp
# PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/dfs.hpp
# PRIVATE ${PROJECT_SOURCE_DIR}/src/id_manager.cpp
# PRIVATE ${PROJECT_SOURCE_DIR}/src/list_graph.i.hpp
# PRIVATE ${PROJECT_SOURCE_DIR}/src/matrix_graph.i.hpp
# PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms/bfs.i.hpp
# PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms/dfs.i.hpp
# )
target_sources(graph
        PUBLIC ${INC}
        PUBLIC ${INC_ALG}
        PRIVATE ${SRC}
        PRIVATE ${SRC_ALG}
)

target_include_directories(graph
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms
)

target_compile_options(graph
        PRIVATE ${COMPILER_FLAGS}
        PRIVATE $<$<BOOL:${COVERAGE}>:--coverage>
)

target_link_options(graph
        PRIVATE ${LINKER_FLAGS}
)

# set_target_properties(graph PROPERTIES LINKER_LANGUAGE CXX)

# ###############################################################################
# COVERAGE da testare
# ###############################################################################
if(COVERAGE)
        # GET GCC MAJOR VERSION
        string(REGEX REPLACE "\\..*" "" GCC_MAJOR ${CMAKE_CXX_COMPILER_VERSION})

        add_custom_target(coverage
                COMMAND gcovr
                --gcov-executable gcov-${GCC_MAJOR}
                --root ${PROJECT_SOURCE_DIR}
                -v
                --html --html-details coverage.html
                COMMENT "Generating Code Coverage"
        )
endif()

# ###############################################################################
# TIDY da testare
# ###############################################################################
add_custom_target(tidy
        COMMAND clang-tidy ${INC} ${INC_ALG} ${SRC} ${SRC_ALG} -checks=*
)

# ###############################################################################
# DOCUMENTATION da testare
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
# TEST CODE
# ###############################################################################
add_executable(graph_test)

target_sources(graph_test
        PRIVATE ${PROJECT_SOURCE_DIR}/test/test_main.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/digraph_test.cpp
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

# COMPILE ON MACOS
# cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-12 ..