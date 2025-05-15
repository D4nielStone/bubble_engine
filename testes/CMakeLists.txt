cmake_minimum_required(VERSION 3.10)
project(testes)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ./include
    ./../
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

target_link_libraries(testes PRIVATE ../libs/glad/glad.c)

# Detectar plataforma
if(WIN32)
    message(STATUS "Configuring for Windows")

    # Assumir que o vcpkg foi integrado via CMAKE_TOOLCHAIN_FILE
    # Inclui apenas diretórios locais e relativos
    include_directories(
        ${CMAKE_SOURCE_DIR}
        ../becommons/include
        ./include
        ../libs/glad
        ../libs/LuaBridge
    )

else()
    message(STATUS "Configuring for Linux")

target_include_directories(testes PRIVATE 
    ../becommons/include
    /usr/include/lua5.3
    /usr/include/glm
    /usr/include/GLFW
    ../libs/glad
    ../libs/LuaBridge
    /usr/include/assimp
    /usr/include
    /usr/include/rapidjson
    /usr/include/bullet
    /usr/include/freetype2
)
endif()

# Linkagem
if(WIN32)
target_link_libraries(testes PRIVATE
    becommons
    lua
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
else()
target_link_libraries(testes PRIVATE 
 ${CMAKE_SOURCE_DIR}/../becommons/out/libbecommons.a 
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
endif()


find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
find_library(BULLET_COLLISION_LIBRARY BulletCollision)
find_library(BULLET_LINEAR_MATH_LIBRARY LinearMath)

add_definitions(-D_DEBUG)

