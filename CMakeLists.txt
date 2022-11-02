cmake_minimum_required(VERSION 3.22)
project(graph_lib
        DESCRIPTION "Graph library"
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # clang-tidy

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/demos/simple_interactive.cpp)

target_include_directories(${PROJECT_NAME}
                           PUBLIC include/
                           PUBLIC include/algorithms
                           PRIVATE src/
                           PRIVATE src/algorithms)

target_sources(${PROJECT_NAME}
               PUBLIC include/g_base.hpp
               PUBLIC include/g_concepts.hpp
               PUBLIC include/g_list_graph.hpp
               PUBLIC include/g_matrix_graph.hpp
               PUBLIC include/algorithms/g_algorithms_base.hpp
               PUBLIC include/algorithms/g_bfs.hpp
               PUBLIC include/algorithms/g_dfs.hpp
               PRIVATE src/g_list_graph.i.hpp
               PRIVATE src/g_matrix_graph.i.hpp
               PRIVATE src/algorithms/g_bfs.i.hpp
               PRIVATE src/algorithms/g_dfs.i.hpp)

#COMMANDS:
# mkdir build
# cd build
# cmake ..
# cmake --build .