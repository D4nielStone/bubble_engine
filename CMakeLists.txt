cmake_minimum_required(VERSION 3.20)

project(
    BubbleEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(BGUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGUI_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/cpp-bgui)

# The reusable engine library.
add_subdirectory(commons)

# Small programs used to exercise the engine.
add_subdirectory(examples)
