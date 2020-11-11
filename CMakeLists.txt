cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) #C++ standard 17 requires cmake 3.8 min
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (Embedded_CV)

#add_executable(Embedded_CV ./src/main.c)
add_subdirectory(src)

# This is where the if statement that would
# check an input variable to optionally build
# the blob_detect example
add_executable(Blob_Detect ./examples/blob_detect_example.c)
add_library(libjpeg mysqrt.cxx)
add_subdirectory(examples)


