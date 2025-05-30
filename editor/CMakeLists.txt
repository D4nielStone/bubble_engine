cmake_minimum_required(VERSION 3.28)
project(editor)

set(CMAKE_CXX_STANDARD 20)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(editor 
    src/util/runtime.cpp
    src/sistemas/editor.cpp
    src/sistemas/gerenciador_projetos.cpp
    src/main.cpp
)

add_dependencies(editor embed_runtime)

target_include_directories(editor PRIVATE 
    ./
    ../becommons/include
    ../
    ./include
    /usr/include/lua5.3
    /usr/include/glm
    /usr/include/GLFW
    ../libs/glad
    ../libs/sol2/include
    /usr/include/assimp
    /usr/include
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)

target_link_libraries(editor PRIVATE 
 ${CMAKE_BINARY_DIR}/bin/becommons/libbecommons.a
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

