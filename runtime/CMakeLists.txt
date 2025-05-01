cmake_minimum_required(VERSION 3.28)
project(runtime)

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
    )

else()
    message(STATUS "Configuring for Linux")

    include_directories(
        ${CMAKE_SOURCE_DIR}
        ../becommons/include
        ./include
        ../libs/glad
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
endif()


find_library(BULLET_DYNAMICS_LIBRARY BulletDynamics)
find_library(BULLET_COLLISION_LIBRARY BulletCollision)
find_library(BULLET_LINEAR_MATH_LIBRARY LinearMath)


# Adicionar definições para o modo de Debug
add_definitions(-D_DEBUG)

# Definir flags de compilação no modo Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ")
