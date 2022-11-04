cmake_minimum_required(VERSION 3.22)

project(GraphLib
        DESCRIPTION "Graph library"
        #VERSION 1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clang-tidy

#add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/demos/simple_interactive.cpp)

################################################################################
# LIBRARY
################################################################################

add_library(graph_lib OBJECT)

target_sources(graph_lib
        PUBLIC ${PROJECT_SOURCE_DIR}/include/g_base.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/g_concepts.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/g_list_graph.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/g_matrix_graph.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/g_algorithms_base.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/g_bfs.hpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms/g_dfs.hpp
        PRIVATE ${PROJECT_SOURCE_DIR}/src/g_list_graph.i.hpp
        PRIVATE ${PROJECT_SOURCE_DIR}/src/g_matrix_graph.i.hpp
        PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms/g_bfs.i.hpp
        PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms/g_dfs.i.hpp
)

target_include_directories(graph_lib
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PUBLIC ${PROJECT_SOURCE_DIR}/include/algorithms
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        PRIVATE ${PROJECT_SOURCE_DIR}/src/algorithms
)

set_target_properties(graph_lib PROPERTIES LINKER_LANGUAGE CXX)

#-------------------------------------------------------------------------------

find_package(Doxygen)
if (DOXYGEN_FOUND)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/doc)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
        COMMENT "Generating API documentation with Doxygen"
    )
endif()


################################################################################
# TEST CODE
################################################################################

add_executable(graph_lib_test)

target_sources(graph_lib_test
        PRIVATE ${PROJECT_SOURCE_DIR}/test/test_main.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/test/digraph_test.cpp
        PRIVATE ${PROJECT_SOURCE_DIR}/third_party/catch2/catch.hpp
)

target_include_directories(graph_lib_test
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(graph_lib_test SYSTEM
	PUBLIC ${PROJECT_SOURCE_DIR}/third_party/catch2
)

target_link_libraries(graph_lib_test
	PUBLIC graph_lib
)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

install(
    FILES       ${CMAKE_BINARY_DIR}/graph_lib_test
    DESTINATION ${PROJECT_SOURCE_DIR}/bin
)


################################################################################
# TESTS
################################################################################

enable_testing()

add_test(NAME digraphTest
        COMMAND graph_lib_test
)

#COMMANDS:
# mkdir build
# cd build
# cmake ..
# cmake --build .
# ninja test