#!/bin/bash
### Check for build dir, if found delete
#[ -d "./build" ] && cd "./build" && [ -e "Makefile" ] && make clean && cd ".." && rm -rf "./build"
[ -d "./build" ] && rm -rf "./build"

printf "\nClean Script Finished\n\n"
