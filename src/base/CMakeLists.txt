add_library(base_methods STATIC base_methods.c)
target_include_directories(base_methods PUBLIC ./inc/)
#This is required for anything that uses math.h "m" is really libm is a library that typically exists by ading -lm flag to gcc 
target_link_libraries(base_methods m)

