cmake_minimum_required(VERSION 3.10)
project(becommons)

set(CMAKE_CXX_STANDARD 20)

# Detectar plataforma
if(WIN32)
    message(STATUS "Configuring for Windows")

    # Assumir que o vcpkg foi integrado via CMAKE_TOOLCHAIN_FILE
    # Inclui apenas diretórios locais e relativos
    include_directories(
        ${CMAKE_SOURCE_DIR}
        ./include
        ../libs/sol2/include
        ../libs/glad
    )

else()
    message(STATUS "Configuring for Linux")

    include_directories(
        ${CMAKE_SOURCE_DIR}
        ./include
        ../libs/glad        
        ../libs/sol2/include
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

# Encontrar pacotes
find_package(Lua REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(Bullet REQUIRED)
find_package(Freetype REQUIRED)

add_library(becommons STATIC)

target_include_directories(becommons PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Fontes do projeto
target_sources(becommons PRIVATE
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
    src/sistemas/sistema_de_interface.cpp
    src/sistemas/sistema_de_renderizacao.cpp
    src/nucleo/projeto.cpp
    src/sistemas/bubble_gui.cpp
    src/util/malha.cpp
    src/util/skybox.cpp
    src/util/material.cpp
    src/os/janela.cpp
)

# Linkagem
if(WIN32)

target_link_libraries(becommons
    PRIVATE
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
    Bullet::BulletCollision
    Bullet::LinearMath
    )
else()
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

endif()
# Instalação
install(TARGETS becommons DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
