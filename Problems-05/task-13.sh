#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "You must pass only two arguments"       
	exit 1
fi

INPUT="${1}"
if [ ! -d "${INPUT}" ]; then
	echo "The first parameter is not a directory"
	exit 2
fi

if [ $# -eq 2 ]; then 
	OUTPUT="${2}"
	if [ ! -d "${OUTPUT}" ]; then
		echo "The second parameter is not a directory"
		exit 3
	fi
else
	OUTPUT="$(date +"%d-%m-%Y")"
	mkdir "${OUTPUT}"
fi

find "${INPUT}" -type f -mmin -45 2>/dev/null -exec cp {} "${OUTPUT}/" \; 

exit 0
