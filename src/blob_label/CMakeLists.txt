add_library(blob_label_methods STATIC ./blob_label_methods.c)
#any .h files needed are pointed to using "target_include_directories" (point to folders holding the .h files)
target_include_directories(blob_label_methods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
target_include_directories(blob_label_methods PUBLIC ${Embedded_CV_SOURCE_DIR}/inc/)
#any separate library must also be linked along with pointed to
target_link_libraries (blob_label_methods LINK_PUBLIC base_methods)
