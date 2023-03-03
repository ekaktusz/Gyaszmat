cmake_minimum_required(VERSION 3.25)

# git submodule add https://github.com/microsoft/vcpkg vendor/vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(gyaszmat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/candle/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tmxlite/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/effolkronium/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/gui/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/audio/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/map/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/parallax/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/resource/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/state/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/animation/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/bagla-engine/particles/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/bagla-engine/audio/*.h
    src/bagla-engine/audio/*.cpp
    src/bagla-engine/gui/*.h
    src/bagla-engine/gui/*.cpp
    src/bagla-engine/map/*.h
    src/bagla-engine/map/*.cpp
    src/bagla-engine/resource/*.h
    src/bagla-engine/resource/*.cpp
    src/bagla-engine/parallax/*.h
    src/bagla-engine/parallax/*.cpp
    src/bagla-engine/state/*.h
    src/bagla-engine/animation/*.h
    src/bagla-engine/animation/*.cpp
    src/bagla-engine/particles/*.h
    src/bagla-engine/particles/*.cpp
)

add_executable(gyaszmat ${SOURCES})

if(WIN32)
  set(TMXLITE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/vendor/tmxlite/lib/libtmxlite-s-d.lib")
  set(CANDLE_LIB "${CMAKE_CURRENT_LIST_DIR}/vendor/candle/lib/Candle-s.lib")
else()
  set(TMXLITE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/vendor/tmxlite/lib/libtmxlite-d.so")
  set(CANDLE_LIB "${CMAKE_CURRENT_LIST_DIR}/vendor/candle/lib/libCandle-s.a")
endif()

set(TMXLITE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/tmxlite/include/tmxlite/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/vendor/tmxlite/module/")

find_package(TMXLITE REQUIRED)
find_package(SFML COMPONENTS system window graphics audio network CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_precompile_headers(gyaszmat PUBLIC "src/pch.h")

target_link_libraries(gyaszmat PRIVATE ${CANDLE_LIB})
target_link_libraries(gyaszmat PRIVATE ${TMXLITE_LIBRARIES})
target_link_libraries(gyaszmat PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(gyaszmat PRIVATE spdlog::spdlog spdlog::spdlog_header_only)