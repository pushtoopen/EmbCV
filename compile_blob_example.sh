#!/bin/bash
#this is a bash script to automatically handle properly building this example.
#a CMAKE and a make
### Check for dir, if not found create it using the mkdir ##
[ ! -d "./build" ] && mkdir -p "build"

#run cmake for blob_example build. BUILD_STATIC and BUILD_EXECUTABLES option flags are required for libjpeg use. Said flags are not needed if libjpeg is not used.
cd ./build
printf "\nRunning CMAKE\n\n"
#this runs cmake with 4 flags, all flags in cmake must be preceded with a "-D":
#1. BUILD_BUILD_BLOB_EXAMPLE=ON: This is the option we made in our cmake file to do all the additional work if we wanted to test blob_labelling in linux (which is what this exaample is)
#2. BUILD_STATIC=ON: This is a flag for libjpeg_turbo so that the libraries are statically built for reference. This means it builds the libjpeg_turbo libraries regardless if they are used. This is done for saftety of build and ensures everything works when this library is referenced.
#3. BUILD_EXECUTABLES=OFF: This is another flag for libjpeg_turbo, it does not build executables that are reference-able by other programs, this is because we're referencing the actual functions within libjpeg_turbo instead of building it as a standalone program and running it/capturing its output
#4. CMAKE_BUILD_TYPE=Debug: This tells CMAKE to build the extra info needed to be able to run gdb or other debug programs on our executable. to build without these, set DCMAKE_BUILD_TYPE=Release or MinSizeRel or RelWithDebInfo
# we store the comand in a variable and execute it all at once using the eval command
command="cmake .. -DBUILD_BLOB_EXAMPLE=ON -DBUILD_STATIC=ON -DBUILD_EXECUTABLES=OFF -DCMAKE_BUILD_TYPE=Debug"
eval $command

#configure libjpeg and generate the included files used. Again this is only needed for libjpeg use:
#these were needed for libjpeg, but not libjpeg_turbo
#cd ../examples/helpers/libjpeg-turbo/
#printf "\nConfiguring libjpeg\n\n"
#./configure CFLAGS='-O2' --disable-shared 
#./configure --prefix="$PWD/build"

#build the project from the makefile generated by cmake:
printf "\nRunning MAKE in the build directory\n\n"
#cd ./build/
make VERBOSE=1

