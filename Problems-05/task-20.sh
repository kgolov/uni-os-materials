#!/bin/bash

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Invalid arguments - you must supply a folder"
	exit 1
fi

ALL=""
DIRNAME=""
while [ -n "${1}" ]; do
	if [ "${1}" = "-a" ] && [ -z "${ALL}" ]; then
		ALL=1
	elif [ -d "${1}" ] && [ -z "${DIRNAME}" ]; then
		DIRNAME="${1}"
	else
		echo "Invalid arguments"
		exit 1
	fi

	shift 1
done

# Firstly find directories
while read DIR; do
	# If the directory is hidden, skip it if we have not received a "-a" flag
	if (basename "${DIR}" | egrep -q "^\.") && [ -z "${ALL}" ]; then
		continue
	fi

	COUNT=""
	# Find all files and directories in that directory
	if [ -z "${ALL}" ]; then
		COUNT=$(find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null -printf "%f\n" | egrep "^[^\.]+" | wc -l)
	else 
		COUNT=$(find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null -printf "%f\n" | egrep -v "^\.$" | wc -l)
	fi

	echo "$(basename "${DIR}") (${COUNT} entries)"
done < <(find "${DIRNAME}" -mindepth 1 -maxdepth 1 2>/dev/null -type d)

# Then, read files
while read FILE; do
	# If the file is hidden, skip it if we have no "-a" flag
	if (basename "${FILE}" | egrep -q "^.") && [ -z "${ALL}" ]; then 
		continue
	fi

	SIZE=$(stat ${FILE} -c "%s")
	
	echo "$(basename "${FILE}") (${SIZE} bytes)"
done < <(find "${DIRNAME}" -mindepth 1 -maxdepth 1 2>/dev/null -type f)

exit 0
