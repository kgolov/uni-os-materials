#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Need file with books"
	exit 1
fi

FILE="${1}"

if [ ! -r "${FILE}" ]; then
	echo "File is not readable"
	exit 2
fi

COUNTER=1
while read line; do
	echo "${COUNTER}.$(echo "${line}" | cut -d "-" -f 2-)"
	COUNTER=$((COUNTER+1))
done < "${FILE}" | sort -t '.' -k 2
