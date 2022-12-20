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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/audio/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/map/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/parallax/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/resource/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/state/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/audio/*.h
    src/audio/*.cpp
    src/gui/*.h
    src/gui/*.cpp
    src/map/*.h
    src/map/*.cpp
    src/resource/*.h
    src/resource/*.cpp
    src/parallax/*.h
    src/parallax/*.cpp
    src/state/*.h
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