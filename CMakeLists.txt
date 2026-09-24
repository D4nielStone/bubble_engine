cmake_minimum_required(VERSION 3.20)

project(
    BubbleEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

# The reusable engine library.
add_subdirectory(commons)

# Small programs used to exercise the engine.
add_subdirectory(examples)
