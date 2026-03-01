cmake_minimum_required(VERSION 4.1)
project(Einstein_s_Playground)

set(CMAKE_CXX_STANDARD 20)

add_executable(Einstein_s_Playground main.cpp)

target_link_libraries(Einstein_s_Playground
        user32
        gdi32
        comctl32
        uxtheme
)