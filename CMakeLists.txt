cmake_minimum_required(VERSION 3.20)
project(audiomixer)

set(CMAKE_CXX_STANDARD 23)
add_definitions(-DUNICODE)

add_executable(audiomixer WIN32 main.cpp safe_release.h audiomixer.h audiomixer.cpp penbrush.h)

target_link_libraries(audiomixer winmm.lib)
