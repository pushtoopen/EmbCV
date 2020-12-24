add_library(blob_label_methods STATIC ./blob_label_methods.c)
target_include_directories(blob_label_methods PUBLIC ./inc/)
target_link_libraries (blob_label_methods LINK_PUBLIC base_methods)
