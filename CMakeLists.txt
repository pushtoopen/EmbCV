cmake_minimum_required (VERSION 3.8)
set(CMAKE_C_STANDARD 11) 
set(CMAKE_C_STANDARD_REQUIRED True)
project (Embedded_CV DESCRIPTION "CMAKE FOR EMBCV EXAMPLES" LANGUAGES C)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE}) 


#used in all examples or usage as a whole
add_subdirectory(${Embedded_CV_SOURCE_DIR}/src/base/)


#build options
option(BUILD_BLOB_EXAMPLE "Optional Blob_Label example build" OFF) #defaults to OFF use cmake -DBUILD_BLOB_EXAMPLE = ON to turn on


#runs based on build options
if(BUILD_BLOB_EXAMPLE)
  #add_subdirectory tells where other CMakeLists.txt files are
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/src/blob_label/)
  #add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/helpers/libjpeg/)
  #need to install nasm compiler, do a sudo apt install nasm in ubuntu
  set(CMAKE_ASM_NASM_COMPILER /usr/bin/nasm)
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/helpers/libjpeg-turbo/)
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/helpers/print_to_csv/)

  #done last because this CMakelists.txt file creates the executable and links the libraries.
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/blob_label/)   
  


 
endif(BUILD_BLOB_EXAMPLE)

# tip: any relative folder path is prepended with " ${CMAKE_CURRENT_SOURCE_DIR}" as "/./" or "." or "pwd"
# tip: any absolute folder path is prepended with " ${Embedded_CV_SOURCE_DIR}" which is the same as the folder with the base CMakeLists.txt file is located. which is created based on the project name created by the project command on line 4.

#base_methods.h is used across many other example methods, thus "include_directories" can be used instead of "target_include_directories" in a specific CMakeLists.txt library file (this project is configured to expect each library/executable to reference their own include files in their own CMakeLists.txt.)
#include_directories(${Embedded_CV_SOURCE_DIR}/src/base/inc/)


