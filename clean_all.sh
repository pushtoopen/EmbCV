#!/bin/bash
### Check for dir, if not found create it using the mkdir ##
[ -d "./build" ] && cd "./build"
[ -e "Makefile" ] && make clean
cd ..
[ -d "./build" ] && rm -rf "./build"

