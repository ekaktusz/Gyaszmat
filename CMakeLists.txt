cmake_minimum_required(VERSION 3.25)

# git submodule add https://github.com/microsoft/vcpkg vendor/vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(gyaszmat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)

set(CANDLE_LIB "${CMAKE_CURRENT_LIST_DIR}/vendor/candle/lib/Candle-s.lib")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/candle/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tmxlite/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/gui/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/audio/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/map/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/parallax/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/resource/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/state/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla/animation/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/bagla/audio/*.h
    src/bagla/audio/*.cpp
    src/bagla/gui/*.h
    src/bagla/gui/*.cpp
    src/bagla/map/*.h
    src/bagla/map/*.cpp
    src/bagla/resource/*.h
    src/bagla/resource/*.cpp
    src/bagla/parallax/*.h
    src/bagla/parallax/*.cpp
    src/bagla/state/*.h
    src/bagla/animation/*.h
    src/bagla/animation/*.cpp
)

add_executable(gyaszmat ${SOURCES})

# TMXlite is trash, the vcpkg support is poopoo
set(TMXLITE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/vendor/tmxlite/lib/libtmxlite-s-d.lib")
set(TMXLITE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/tmxlite/include/tmxlite/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/vendor/tmxlite/module/")
find_package(TMXLITE REQUIRED)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_precompile_headers(gyaszmat PUBLIC "src/pch.h")

target_link_libraries(gyaszmat PRIVATE ${CANDLE_LIB})
target_link_libraries(gyaszmat PRIVATE ${TMXLITE_LIBRARIES})
target_link_libraries(gyaszmat PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(gyaszmat PRIVATE spdlog::spdlog spdlog::spdlog_header_only)