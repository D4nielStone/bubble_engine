# Diretório de saída para binários
cmake_minimum_required(VERSION 3.28)
project(bubble_engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Subdiretórios
add_subdirectory(${CMAKE_SOURCE_DIR}/becommons)
add_subdirectory(${CMAKE_SOURCE_DIR}/runtime)
add_subdirectory(${CMAKE_SOURCE_DIR}/editor)
add_subdirectory(${CMAKE_SOURCE_DIR}/testes)
