cmake_minimum_required(VERSION 3.28)

project(editor LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# CONFIGURAÇÃO MSYS2 UCRT64
# ==============================================================================

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Configuring Windows with MinGW/UCRT64")

    if(DEFINED ENV{MSYSTEM})
        message(STATUS "MSYS2 environment: $ENV{MSYSTEM}")
    endif()

    if(DEFINED ENV{MSYSTEM_PREFIX})
        message(STATUS "MSYS2 prefix: $ENV{MSYSTEM_PREFIX}")
    endif()
endif()

# ==============================================================================
# ENCONTRAR PACOTES
# ==============================================================================

find_package(Lua REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(assimp CONFIG REQUIRED)

find_package(RapidJSON CONFIG REQUIRED)

find_package(Bullet REQUIRED
    COMPONENTS
        BulletDynamics
        BulletCollision
        LinearMath
)

find_package(Freetype REQUIRED)

# ==============================================================================
# DEBUG DOS PACOTES
# ==============================================================================

message(STATUS "----------------------------------------")
message(STATUS "FreeType")
message(STATUS "  Found: ${Freetype_FOUND}")
message(STATUS "  Version: ${Freetype_VERSION_STRING}")
message(STATUS "  Include dirs: ${FREETYPE_INCLUDE_DIRS}")
message(STATUS "  Libraries: ${FREETYPE_LIBRARIES}")
message(STATUS "----------------------------------------")

# ==============================================================================
# CRIAÇÃO DO EXECUTÁVEL
# ==============================================================================

add_executable(editor
    src/util/runtime.cpp
    src/sistemas/editor.cpp
    src/sistemas/gerenciador_projetos.cpp
    src/main.cpp
)

# ==============================================================================
# RUNTIME
# ==============================================================================

if(BUILD_RUNTIME)
    add_dependencies(editor embed_runtime)
endif()

# ==============================================================================
# DIRETÓRIOS DE INCLUSÃO
# ==============================================================================

target_include_directories(editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${BULLET_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
)

# ==============================================================================
# DEFINIÇÕES
# ==============================================================================

target_compile_definitions(editor PRIVATE
    SOL_ALL_SAFETIES_ON=1
    SOL_LUA_VERSION=504
)

# ==============================================================================
# LINKAGEM
# ==============================================================================

target_link_libraries(editor PRIVATE

    # Projeto
    becommons

    # GLFW
    glfw

    # FreeType
    Freetype::Freetype

    # Assimp
    assimp::assimp

    # Bullet
    ${BULLET_LIBRARIES}
)

# ==============================================================================
# BIBLIOTECAS NATIVAS DO WINDOWS
# ==============================================================================

if(WIN32)

    target_link_libraries(editor PRIVATE
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

# ==============================================================================
# LINKAGEM ESTÁTICA DO RUNTIME C/C++
# ==============================================================================

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_link_options(editor PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )

endif()