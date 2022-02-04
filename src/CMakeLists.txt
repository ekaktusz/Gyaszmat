cmake_minimum_required(VERSION 3.5)

if(WIN32) 
    set(CMAKE_C_COMPILER_WORKS TRUE)
    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/../include/SFML/lib/cmake/SFML")
elseif(UNIX)
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
else()
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    set(CMAKE_C_COMPILER "/usr/bin/clang")
endif()

project(gyaszmat)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML 2.5 COMPONENTS system graphics audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(gyaszmat ${SOURCES})
target_link_libraries(gyaszmat PUBLIC sfml-system sfml-window sfml-graphics sfml-audio)
