cmake_minimum_required(VERSION 3.0.0)
project(next_dwm_status VERSION 0.1.0)

find_package(Git QUIET)
function(add_git_submodule dir)
    if(GIT_FOUND)
        message(STATUS "start update submodule " ${dir})
        # 下载git子模块
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${dir}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            # 下载失败的处理
            message(WARNING "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        else()
            add_subdirectory(${dir})
        endif()
    else()
        message(WARNING "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endfunction(add_git_submodule)
add_git_submodule(3rdparty/fileWatcher)

add_compile_options(-g)

include_directories(include)
include_directories(3rdparty/fileWatcher/include)

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.local)

add_library(util_volume src/lib/volume.cpp)
target_compile_options(util_volume PRIVATE -std=c++17)
target_link_libraries(util_volume asound)

# apt install libasound2-dev
add_executable(next_dwm_status src/exe/status.cpp)
target_compile_options(next_dwm_status PRIVATE -std=c++14)
target_link_libraries(next_dwm_status X11 pthread util_volume file_watcher)

# test program
add_executable(volume_test src/exe/volume_test.cpp)
target_link_libraries(volume_test util_volume)

install(TARGETS next_dwm_status)
