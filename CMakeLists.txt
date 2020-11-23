cmake_minimum_required (VERSION 3.8)
set(CMAKE_C_STANDARD 11) 
set(CMAKE_C_STANDARD_REQUIRED True)
project (Embedded_CV DESCRIPTION "CMAKE FOR EMBCV EXAMPLES" LANGUAGES C)

#used in all examples or usage as a whole
add_subdirectory(${Embedded_CV_SOURCE_DIR}/src/base/)

#build options
option(BUILD_BLOB_EXAMPLE "Optional Blob_Label example build" OFF) #defaults to OFF use cmake -DBUILD_BLOB_EXAMPLE = ON to turn on





#runs based on build options
if(BUILD_BLOB_EXAMPLE)
	#add_subdirectory tells where other CMakeLists.txt files are
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/src/blob_label/)
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/helpers/print_to_csv/)
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/helpers/libjpeg/)
  add_subdirectory(${Embedded_CV_SOURCE_DIR}/examples/blob_label/)
  add_executable(Blob_Label ${Embedded_CV_SOURCE_DIR}/examples/blob_label/blob_label_example.c)
  #
  #find_package (jpeg REQUIRED)
  #target_link_libraries(Embedded_CV jpeg::jpeg)
  
  install(TARGETS Blob_Label DESTINATION bin)
endif(BUILD_BLOB_EXAMPLE)






