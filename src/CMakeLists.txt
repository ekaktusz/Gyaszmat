cmake_minimum_required(VERSION 3.5)

set(CMAKE_DEBUG_POSTFIX d)

if(WIN32)
    set(CMAKE_C_COMPILER_WORKS TRUE)
    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/../include/SFML/lib/cmake/SFML")
else()
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    set(CMAKE_C_COMPILER "/usr/bin/clang")
endif()

set(TMXLITE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../lib/tmxlite/libtmxlite-d.so")
set(TMXLITE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../include/tmxlite/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/../cmake/modules/")

project(gyaszmat)

set(CMAKE_CXX_STANDARD 20)

find_package(TMXLITE REQUIRED)
find_package(SFML 2.5 COMPONENTS system graphics audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR} "${CMAKE_CURRENT_LIST_DIR}/../include/")

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(gyaszmat ${SOURCES})
set_target_properties(gyaszmat PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(gyaszmat PUBLIC sfml-system sfml-window sfml-graphics sfml-audio ${TMXLITE_LIBRARIES})
