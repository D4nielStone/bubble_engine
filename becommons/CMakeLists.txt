cmake_minimum_required(VERSION 3.20)

project(becommons LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 AND NOT MSVC)
    message(STATUS "Configuring for Windows (UCRT64/MinGW)")
endif()

find_package(Lua 5.4 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Freetype REQUIRED)
find_package(Bullet REQUIRED COMPONENTS
    BulletDynamics
    BulletCollision
    LinearMath
)

find_path(
    FREEIMAGE_INCLUDE_DIR
    NAMES FreeImage.h
    REQUIRED
)

find_library(
    FREEIMAGE_LIBRARY
    NAMES freeimage FreeImage
    REQUIRED
)

message(STATUS "Lua version: ${LUA_VERSION_STRING}")
message(STATUS "Lua include dir: ${LUA_INCLUDE_DIR}")
message(STATUS "Lua libraries: ${LUA_LIBRARIES}")

add_library(becommons STATIC)

target_include_directories(becommons
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/sol2/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libs/glad
        ${BULLET_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${FREEIMAGE_INCLUDE_DIR}
)

target_sources(becommons
    PRIVATE
        src/api/api_lua.cpp
        ../libs/glad/glad.c
        src/arquivadores/fonte.cpp
        src/nucleo/registro.cpp
        src/arquivadores/imageloader.cpp
        src/arquivadores/shader.cpp
        src/arquivadores/modelo.cpp
        src/componentes/camera.cpp
        src/componentes/camera_editor.cpp
        src/componentes/luz_direcional.cpp
        src/componentes/codigo.cpp
        src/componentes/fisica.cpp
        src/componentes/transformacao.cpp
        src/componentes/terreno.cpp
        src/inputs/inputs.cpp
        src/nucleo/fase.cpp
        src/sistemas/sistema_de_codigo.cpp
        src/sistemas/sistema_de_fisica.cpp
        src/sistemas/sistema_de_renderizacao.cpp
        src/sistemas/interface.cpp
        src/nucleo/projeto.cpp
        src/util/malha.cpp
        src/util/skybox.cpp
        src/util/material.cpp
        src/os/janela.cpp
)

target_link_libraries(becommons
    PUBLIC
        ${LUA_LIBRARIES}
    PRIVATE
        glfw
        assimp::assimp
        ${FREEIMAGE_LIBRARY}
        Freetype::Freetype
        ${BULLET_LIBRARIES}
)

target_include_directories(becommons
    SYSTEM PUBLIC
        ${LUA_INCLUDE_DIR}
)

if(WIN32)
    target_link_libraries(becommons
        PRIVATE
            opengl32
            gdi32
            winmm
            imm32
            ole32
            uuid
            version
            setupapi
    )
endif()

install(
    TARGETS becommons
    DESTINATION lib
)

install(
    DIRECTORY include/
    DESTINATION include
)