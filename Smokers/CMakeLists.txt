cmake_minimum_required(VERSION 3.10)
project(Smokers)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(smokers
src/main.c
)

target_link_libraries(smokers pthread)