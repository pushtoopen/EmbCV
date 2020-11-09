cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) #C++ standard 17 requires cmake 3.8 min

project (Embedded_CV)

add_executable(Embedded_CV ./src/main.c)
add_subdirectory(src)
