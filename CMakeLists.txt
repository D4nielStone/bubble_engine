cmake_minimum_required(VERSION 3.28)
project(bubble_engine)

set(OUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Opção para compilar o runtime
option(BUILD_RUNTIME "Compilar o runtime" OFF)

# Subdiretórios
add_subdirectory(${CMAKE_SOURCE_DIR}/becommons ${OUT_DIR}/becommons)
# Compilar runtime apenas se BUILD_RUNTIME for ON
if(BUILD_RUNTIME)
    add_subdirectory(${CMAKE_SOURCE_DIR}/testes ${OUT_DIR}/testes)

    add_subdirectory(${CMAKE_SOURCE_DIR}/runtime ${OUT_DIR}/runtime)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/editor ${OUT_DIR}/editor)
