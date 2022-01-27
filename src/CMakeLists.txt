cmake_minimum_required(VERSION 3.5)

project(gyaszmat)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS system graphics audio REQUIRED)

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(gyaszmat ${SOURCES})
target_link_libraries(gyaszmat sfml-system sfml-graphics sfml-audio)