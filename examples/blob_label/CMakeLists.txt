#This is the CMakeLists file that defines the executable
#Remember that the CMake items are really just to help demonstrate EmbCV capability.
#example of items needed for any executable utilizing the EmbCV methods in linux, not needed in an embedded environment
add_executable(blob_label_example blob_label_example.c ${CMAKE_CURRENT_SOURCE_DIR}/inc/libjpeg_declarations.h)

#library linking happens after an executable has been created
#this is the standard library that EmbCV has the methods within
target_link_libraries (blob_label_example LINK_PUBLIC base_methods)

#example of addons used (helpers are typically not used in non-linux environments)
target_link_libraries (blob_label_example LINK_PUBLIC jpeg)
target_link_libraries (blob_label_example LINK_PUBLIC blob_label_methods)
target_link_libraries (blob_label_example LINK_PUBLIC print_csv)

