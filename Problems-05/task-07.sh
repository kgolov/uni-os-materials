#!/bin/bash

read -p "Enter full path to directory: " dirname

if [ ! -d $dirname ]; then
	echo "$dirname is not a directory!"
	exit 1
else 
	files=$(find $dirname -type f 2> /dev/null | wc -l)
	echo "Number of files: $files"
	dirs=$(find $dirname -type d 2> /dev/null | wc -l)
	echo "Number of dirs: $dirs"
fi
