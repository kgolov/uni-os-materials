#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Please enter a directory and a number"
	exit 1
fi

DIR="${1}"
NUM="${2}"

if [ ! -d "${DIR}" ]; then
	echo "You must supply a directory"
	exit 2
fi

if !(echo "${NUM}" | egrep -q "^[0-9]+$"); then
	echo "You must supply a valid number"
	exit 2
fi

find "${DIR}" -maxdepth 1 -type f -size "+${NUM}c" -printf "%s\n" | awk '{ sum += $1 } END { print sum }'
exit 0
