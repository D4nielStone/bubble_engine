cmake_minimum_required(VERSION 3.28)
project(bubble_engine)

set(OUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Opção para compilar os testes
option(BUILD_TESTS "Compilar testes" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/becommons ${OUT_DIR}/becommons)

    add_subdirectory(${CMAKE_SOURCE_DIR}/runtime ${OUT_DIR}/runtime)
    add_subdirectory(${CMAKE_SOURCE_DIR}/editor ${OUT_DIR}/editor)
if(BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/testes ${OUT_DIR}/testes)
endif()
