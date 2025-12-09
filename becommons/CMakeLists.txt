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
        /usr/include/glm
        /usr/include/GLFW
        /usr/include/assimp
        /usr/include/FreeImage
        ../libs/rapidjson/include
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

add_library(becommons SHARED)

target_include_directories(becommons PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Fontes do projeto
target_sources(becommons PRIVATE
    src/nucleo/projeto.cpp
    src/nucleo/fase.cpp
    src/nucleo/engine.cpp
    src/nucleo/levelmanager.cpp
    ../libs/glad/glad.c
    src/api/api_lua.cpp
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
    src/componentes/renderizador.cpp
    src/inputs/inputs.cpp
    src/sistemas/editor.cpp
    src/sistemas/sistema_de_codigo.cpp
    src/sistemas/sistema_de_fisica.cpp
    src/sistemas/sistema_de_renderizacao.cpp
    src/sistemas/interface.cpp
    src/util/caixa.cpp
    src/util/malha.cpp
    src/util/skybox.cpp
    src/util/material.cpp
    src/elementos/area_de_toque.cpp
    src/elementos/texto.cpp
    src/elementos/popup.cpp
    src/elementos/gerenciador_projetos.cpp
    src/elementos/docking.cpp
    src/elementos/painel.cpp
    src/elementos/area_de_texto.cpp
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
    PUBLIC
        Lua::Lua
        glfw
        glad
        assimp
        freeimage
        freetype
        BulletCollision
        BulletDynamics
        LinearMath
        Bullet3Common
)

endif()
# Instalação
install(TARGETS becommons DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
