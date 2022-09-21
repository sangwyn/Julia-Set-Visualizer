cmake_minimum_required(VERSION 3.23)
project(algebra_fractal)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(algebra_fractal main.cpp Globals.h ComplexNum.cpp ComplexNum.h)

target_link_libraries(algebra_fractal sfml-graphics sfml-audio)