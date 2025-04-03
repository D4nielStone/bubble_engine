cmake_minimum_required(VERSION 3.10)
project(becommons)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /usr/include/lua5.3
    /usr/include/glm
    /usr/include/GLFW
    /usr/include/glad
    /usr/include/assimp
    /usr/include/FreeImage
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)

add_library(becommons STATIC)

target_include_directories(becommons PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Adicionar os arquivos de código fonte da biblioteca
target_sources(becommons PRIVATE
    src/api/api_lua.cpp
    /usr/include/glad/glad.c
    src/arquivadores/fonte.cpp
    src/nucleo/registro.cpp
    src/arquivadores/imageloader.cpp
    src/arquivadores/shader.cpp
    src/arquivadores/modelo.cpp
    src/componentes/camera.cpp
    src/componentes/camera_editor.cpp
    src/componentes/codigo.cpp
    src/componentes/fisica.cpp
    src/componentes/transformacao.cpp
    src/componentes/terreno.cpp
    src/inputs/inputs.cpp
    src/nucleo/fase.cpp
    src/nucleo/sistema_de_codigo.cpp
    src/nucleo/sistema_de_fisica.cpp
    src/nucleo/sistema_de_interface.cpp
    src/nucleo/sistema_de_renderizacao.cpp
    src/nucleo/sistema.cpp
    src/nucleo/projeto.cpp
    src/util/malha.cpp
    src/util/skybox.cpp
    src/os/janela.cpp
)

# Linkar as bibliotecas necessárias
target_link_libraries(becommons
    PRIVATE
    lua5.3
    glfw
    glad 
    assimp
    FreeImage
    rapidjson
    Bullet
    freetype
    BulletCollision
    BulletDynamics
    Bullet3Common
)

# Instalar a biblioteca (se necessário)
install(TARGETS becommons DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)


# Adicionar definições para o modo de Debug
add_definitions(-D_DEBUG)

# Definir flags de compilação no modo Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
