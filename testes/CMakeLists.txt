cmake_minimum_required(VERSION 3.10)
project(testes)

set(CMAKE_CXX_STANDARD 20)

add_library(becommons STATIC IMPORTED)
set_target_properties(becommons PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../becommons/out/libbecommons.a)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../
)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(testes 
    src/registro_testes.cpp
    src/main.cpp
)

add_library(glad STATIC /usr/include/glad/glad.c)
target_link_libraries(testes PRIVATE glad)

target_include_directories(testes PRIVATE 
    ../becommons/include
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

target_link_libraries(testes PRIVATE 
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

#set(CMAKE_BUILD_TYPE Debug)  # Garante que estamos gerando uma build de depuração
#set(CMAKE_CXX_FLAGS_DEBUG "-g")  # Adiciona símbolos de depuração

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()
