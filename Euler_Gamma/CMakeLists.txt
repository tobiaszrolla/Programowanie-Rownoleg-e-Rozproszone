cmake_minimum_required(VERSION 3.10)
project(EulerGamma)

set(CMAKE_C_STANDARD 11)

add_executable(euler_gamma
main.c
)

target_link_libraries(euler_gamma pthread m)