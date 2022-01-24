#!/bin/bash
 read -n 1 -p "Running Blob Label Example, Would you like to debug using gdb? (y/n):" VAR

if [[ $VAR == 'y' ]]
then
    gdb ./build/examples/blob_label/blob_label_example
elif [[ $VAR == 'n' ]]
then
    ./build/examples/blob_label/blob_label_example
else
    exit
fi


