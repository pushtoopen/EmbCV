#This is the CMakeLists file that defines the executable
#Remember that the CMake items are really just to help demonstrate EmbCV capability.
#example of items needed for any executable utilizing the EmbCV methods in linux, not needed in an embedded environment
add_executable(blob_label_example blob_label_example.c)
target_include_directories(blob_label_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/src/base/inc/)
target_include_directories(blob_label_example PUBLIC ${Embedded_CV_SOURCE_DIR}/src/blob_label/inc/)


#library linking happens after an executable has been created
#this is the standard library that EmbCV has the methods within
#any separate library must also be linked along with pointed to
target_link_libraries (blob_label_example LINK_PUBLIC base_methods)

#example of addons used (in this example helpers are typically not used in an embedded environment, they are to help with the example)
target_link_libraries (blob_label_example LINK_PUBLIC blob_label_methods)
target_link_libraries (blob_label_example LINK_PUBLIC print_csv)


add_custom_command(
        TARGET blob_label_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/openimggreyscale.py
                ${CMAKE_CURRENT_BINARY_DIR}/openimggreyscale.py) 


  add_custom_command(
        TARGET blob_label_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/blobs.jpg
                ${CMAKE_CURRENT_BINARY_DIR}/blobs.jpg) 

