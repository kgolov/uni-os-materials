#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must pass 2 parameters - a directory and a number!"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "${1} is not a directory!"
	exit 2
fi

if ! (echo "${2}" | egrep -q "^[0-9]+$"); then
	echo "${2} is not a valid number!"
	exit 3
fi

DIRNAME="${1}"
NUMBER="${2}"

OUTPUT="$(find "${DIRNAME}" -mindepth 1 -maxdepth 1 -type f -size +${NUMBER}c -printf "%f\n")"

if [ -z "${OUTPUT}" ]; then
	echo "No files found larger than ${NUMBER} bytes."
else 
	echo "Files larger than ${NUMBER} bytes:"
	echo "${OUTPUT}"
fi

exit 0
