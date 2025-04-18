cmake_minimum_required(VERSION 3.28)
project(editor)

set(CMAKE_CXX_STANDARD 20)

add_library(becommons STATIC IMPORTED)
set_target_properties(becommons PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../becommons/out/libbecommons.a)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(editor 
    src/sistemas/editor.cpp
    src/sistemas/gerenciador_projetos.cpp
    src/main.cpp
)

add_library(glad STATIC /usr/include/glad/glad.c)
target_link_libraries(editor PRIVATE glad)

target_include_directories(editor PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ../becommons/include
    ../
    ./include
    /usr/include/lua5.3
    /usr/include/glm
    /usr/include/GLFW
    /usr/include/glad
    /usr/include/assimp
    /usr/include
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)

target_link_libraries(editor PRIVATE 
 becommons 
 BulletDynamics
 LinearMath 
 BulletCollision
 Bullet3Common
 glfw 
 assimp
 lua5.3
 freeimage
 freetype
)


find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
find_library(BULLET_COLLISION_LIBRARY BulletCollision)
find_library(BULLET_LINEAR_MATH_LIBRARY LinearMath)


# Adicionar definições para o modo de Debug
add_definitions(-D_DEBUG)

# Definir flags de compilação no modo Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
