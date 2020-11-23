add_library(blob_label_example blob_label_example.c)
target_include_directories(blob_label_example PUBLIC ../helpers/libjpeg/)
target_include_directories(blob_label_example PUBLIC ./inc/)
