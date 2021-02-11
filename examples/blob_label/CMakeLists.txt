#This is the CMakeLists file that defines the executable
#Remember that the CMake items are really just to help demonstrate EmbCV capability.
#example of items needed for any executable utilizing the EmbCV methods in linux, not needed in an embedded environment
add_executable(blob_label_example blob_label_example.c ${CMAKE_CURRENT_SOURCE_DIR}/inc/libjpeg_declarations.h)
#any .h files needed are pointed to using "target_include_directories" (point to folders holding the .h files)
target_include_directories(blob_label_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/src/base/inc/)
target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/src/blob_label/inc/)
#target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/examples/helpers/libjpeg/)
target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/examples/helpers/libjpeg-turbo/)
#library linking happens after an executable has been created
#this is the standard library that EmbCV has the methods within
#any separate library must also be linked along with pointed to
target_link_libraries (blob_label_example LINK_PUBLIC base_methods)

#example of addons used (helpers are typically not used in non-linux environments)
#target_link_libraries (blob_label_example LINK_PUBLIC jpeg)
target_link_libraries (blob_label_example LINK_PUBLIC turbojpeg-static)
target_link_libraries (blob_label_example LINK_PUBLIC blob_label_methods)
target_link_libraries (blob_label_example LINK_PUBLIC print_csv)

  add_custom_command(
        TARGET blob_label_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/blobs.jpg
                ${CMAKE_CURRENT_BINARY_DIR}/blobs.jpg) 

