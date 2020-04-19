#!/bin/bash

while [ -n "${1}" ]; do
	PARAM="${1}"
	if [ -f "${PARAM}" ]; then
		if [ -r "${PARAM}" ]; then
			echo "${PARAM} is a readable file"
		else 
			echo "${PARAM} is a file, but it is not readable"
		fi
	elif [ -d "${PARAM}" ]; then
		COUNT=$(find "${PARAM}" -mindepth 1 -maxdepth 1 -type f | wc -l)
		echo "${PARAM} is a directory with ${COUNT} files. The following files are bigger than ${COUNT} bytes: "

		OUTPUT="$(find "${PARAM}" -mindepth 1 -maxdepth 1 -type f -size +${COUNT}c -printf "%f\n")"
		if [ -z "${OUTPUT}" ]; then
			echo "No files are bigger than ${COUNT} bytes"
		else 
			echo "${OUTPUT}"
		fi
	fi
	
	shift 1
done
