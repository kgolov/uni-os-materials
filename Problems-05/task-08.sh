#!/bin/bash

echo "Please enter the names of 3 files"
read -p "File one: " input1
read -p "File two: " input2
read -p "Output file: " output

if [ ! -r $input1 ] || [ ! -r $input2 ]; then
	echo "Files do not exist or read permissions are denied!"
	exit 1
fi

paste $input1 $input2 | sort -o $output 

echo "Paste completed successfully in file: $output"
