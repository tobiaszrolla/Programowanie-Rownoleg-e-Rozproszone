cmake_minimum_required(VERSION 3.10)
project(Dining_Philospopher)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(philosopher 
src/main.c
src/Fork.c
src/Philosopher.c
)

target_link_libraries(philosopher pthread)