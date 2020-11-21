#!/bin/bash
### Check for build dir, if found delete
#[ -d "./build" ] && cd "./build" && [ -e "Makefile" ] && make clean && cd ".." && rm -rf "./build"
[ -d "./build" ] && rm -rf "./build"
#clean libjpeg
cd ./examples/helpers/libjpeg/
make clean
[ -d "./build" ] && rm -rf "./build"

