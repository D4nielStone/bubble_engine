# Diretório de saída para binários
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Subdiretórios
add_subdirectory(${CMAKE_SOURCE_DIR}/becommos/CMakeLists.txt)
add_subdirectory(${CMAKE_SOURCE_DIR}/runtime)
add_subdirectory(${CMAKE_SOURCE_DIR}/editor)
add_subdirectory(${CMAKE_SOURCE_DIR}/testes)
