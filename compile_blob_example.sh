#!/bin/bash
### Check for dir, if not found create it using the mkdir ##
[ ! -d "./build" ] && mkdir -p "build"

cd ./build
printf "\nRunning CMAKE\n\n"
command="cmake .. -DBUILD_BLOB_EXAMPLE=ON"
eval $command

cd ../examples/helpers/libjpeg/
printf "\nConfiguring libjpeg\n\n"
./configure CFLAGS='-O2' --disable-shared

printf "\nRunning MAKE\n\n"
cd ../../../build/
make
