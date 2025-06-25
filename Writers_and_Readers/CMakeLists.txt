cmake_minimum_required(VERSION 3.10)
project(Writers_and_Readers)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(writers_and_readers
src/main.c
src/Book.c
src/Person.c
)

target_link_libraries(writers_and_readers pthread)