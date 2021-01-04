#!/bin/bash
### Check for dir, if not found create it using the mkdir ##
[ ! -d "./build" ] && mkdir -p "build"

cd ./build
printf "\nRunning CMAKE\n\n"
command="cmake .. -DBUILD_BLOB_EXAMPLE=ON -DBUILD_STATIC=ON -DBUILD_EXECUTABLES=OFF"
eval $command

cd ../examples/helpers/libjpeg/
printf "\nConfiguring libjpeg\n\n"
#./configure CFLAGS='-O2' --disable-shared 
./configure --prefix="$PWD/build"

#make
#make -n install
#make install
#export LD_LIBRARY_PATH="$PWD/build/lib/${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"


printf "\nRunning MAKE in the build directory\n\n"
cd ../../../build/
make VERBOSE=1

