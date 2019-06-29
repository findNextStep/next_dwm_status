cmake_minimum_required(VERSION 3.0.0)
project(next_dwm_status VERSION 0.1.0)

add_compile_options(-g)

include_directories(include)

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local)

add_library(util_volume src/lib/volume.c)
target_link_libraries(util_volume asound)

# apt install libasound2-dev
add_executable(next_dwm_status src/exe/status.cpp)
target_compile_options(next_dwm_status PRIVATE -std=c++14)
target_link_libraries(next_dwm_status X11 pthread util_volume)

install(TARGETS next_dwm_status)