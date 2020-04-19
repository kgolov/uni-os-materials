#!/bin/bash

if [ $# -eq 0 ]; then
	echo "You must supply some files to search in"
	exit 1
fi

read -p "Please enter a string to search for: " NEEDLE

echo -e "Searching for the string: "\"${NEEDLE}\""\n"
while [ -n "${1}" ]; do
	FILE="${1}"

	if [ ! -r "${FILE}" ]; then
		echo "${FILE} is not readable"
		continue
	fi

	COUNT=$(egrep -c -a "${NEEDLE}" "${FILE}")
	echo -e "${FILE}\tcontains:\t${COUNT} matches"
	
	shift 1
done
