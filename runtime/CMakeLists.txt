cmake_minimum_required(VERSION 3.28)
project(runtime)

set(CMAKE_CXX_STANDARD 20)

find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED )
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(runtime 
    src/main.cpp
)

add_library(glad STATIC ../libs/glad/glad.c)
target_link_libraries(runtime PRIVATE glad)

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
        ../libs/LuaBridges
    )

else()
    message(STATUS "Configuring for Linux")

    include_directories(
        ${CMAKE_SOURCE_DIR}
        ../becommons/include
        ./include
        ../libs/glad
        ../libs/LuaBridge
        /usr/include/lua5.3
        /usr/include/glm
        /usr/include/GLFW
        /usr/include/assimp
        /usr/include/FreeImage
        /usr/include/rapidjson
        /usr/include/bullet
        /usr/include/freetype2
    )
endif()

# Linkagem
if(WIN32)
target_link_libraries(runtime PRIVATE
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
target_link_libraries(runtime PRIVATE 
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
endif()


find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
find_library(BULLET_COLLISION_LIBRARY BulletCollision)
find_library(BULLET_LINEAR_MATH_LIBRARY LinearMath)

set(RUNTIME_OUTPUT $<TARGET_FILE:runtime>)
set(RUNTIME_HPP_FILE ${CMAKE_SOURCE_DIR}/../editor/include/assets/runtime_embutido.hpp)

add_custom_command(
    OUTPUT ${RUNTIME_HPP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo ">> Embutindo runtime"
    COMMAND ${CMAKE_COMMAND} -E mkdir ${RUNTIME_HPP_FILE}
    COMMAND xxd -i ${RUNTIME_OUTPUT} ${RUNTIME_HPP_FILE}
    DEPENDS runtime
    COMMENT ">> Runtime embutido."
)

add_custom_target(embed_runtime ALL
    DEPENDS ${RUNTIME_HPP_FILE})
