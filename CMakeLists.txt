cmake_minimum_required(VERSION 3.0.0)
project(next_dwm_status VERSION 0.1.0)

add_compile_options(-std=c++17)
add_compile_options(-g)

include_directories(include)

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local)

# apt install libasound2-dev
add_executable(next_dwm_status src/exe/status.cpp)
target_link_libraries(next_dwm_status X11 pthread asound)

install(TARGETS next_dwm_status)