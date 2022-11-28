cmake_minimum_required(VERSION 3.22)
project(pdc)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
add_executable(pdc main.cpp)

# Add libraries for code completion and compiling
target_link_libraries(pdc PUBLIC MPI::MPI_CXX)