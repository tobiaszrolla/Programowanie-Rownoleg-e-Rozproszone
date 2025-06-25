cmake_minimum_required(VERSION 3.10)
project(MrowczyKomiwojazer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_executable(main main.cpp)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


# Linkowanie przez target MPI::MPI_CXX (dla C++ MPI)
target_link_libraries(main MPI::MPI_CXX m sfml-graphics sfml-window sfml-system)