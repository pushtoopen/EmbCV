#!/bin/bash
 read -n 1 -p "Running Blob Label Example, Would you like to debug using gdb? (y/n):" VAR
cd ./build/examples/blob_label
if [[ $VAR == 'y' ]]
then
    gdb ./blob_label_example
elif [[ $VAR == 'n' ]]
then
    ./blob_label_example
else
    exit
fi


