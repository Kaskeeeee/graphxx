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
                           PRIVATE src/)

target_sources(${PROJECT_NAME}
               PUBLIC include/g_base.hpp
               PUBLIC include/g_concepts.hpp
               PUBLIC include/g_list_digraph.hpp
               PUBLIC include/g_iterator.hpp
               PUBLIC include/g_container.hpp
               PUBLIC include/g_matrix_digraph.hpp
               PRIVATE src/g_list_digraph.cpp
               PRIVATE src/g_matrix_digraph.cpp)

#COMMANDS:
# mkdir build
# cd build
# cmake ..
# cmake --build .