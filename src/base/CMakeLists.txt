add_library(base_methods STATIC ./base_methods.c)
#any .h files needed are pointed to using "target_include_directories" (point to folders holding the .h files)
target_include_directories(base_methods PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
#This is required for anything that uses math.h "m" is really libm is a library that typically exists by ading -lm flag to gcc 
#find_package(m) #sometimes needed if libm isnt installed in a system directory.
target_link_libraries(base_methods m)

