add_library(print_csv print_to_csv.c)
#any .h files needed are pointed to using "target_include_directories" (point to folders holding the .h files)
target_include_directories(print_csv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
target_include_directories(print_csv PUBLIC ${Embedded_CV_SOURCE_DIR}/inc/)
#any separate library must also be linked along with pointed to
target_link_libraries (print_csv LINK_PUBLIC base_methods)


