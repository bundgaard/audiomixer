cmake_minimum_required(VERSION 3.21)
project(audiomixer)

set(CMAKE_CXX_STANDARD 23)

add_executable(audiomixer main.cpp safe_release.h)

target_link_libraries(audiomixer winmm.lib)