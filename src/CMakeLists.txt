cmake_minimum_required(VERSION 3.5)
project(gyaszmat)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS system graphics audio REQUIRED)

add_executable(gyaszmat main.cpp)
target_link_libraries(gyaszmat sfml-system sfml-graphics sfml-audio)