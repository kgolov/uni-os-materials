#!/bin/bash

if [ $# -ne 2 ]; then
	echo "A file and directory are required"
	exit 2
fi

FILE="${1}"
DIR="${2}"

if [ ! -r "${FILE}" ]; then
	echo "${FILE} is not readable"
	exit 3
fi

if [ ! -d "${DIR}" ]; then
	echo "${DIR} is not a directory"
	exit 4
fi

# Calculate input file size (in bytes) and hash, to compare later
INPUT_HASH=$(sha256sum "${FILE}" | cut -d ' ' -f 1)
INPUT_SIZE=$(stat "${FILE}" -c "%s")

# Flag to store if we have found any copies
FLAG=false

# We will search by firstly matching file size, so as to calculate hash for a smaller number of files
# We also want to find files, which are different from the input file itself
while read LINE; do
	CMP_HASH="$(sha256sum "${LINE}" | cut -d ' ' -f 1)"

	if [ "${INPUT_HASH}" = "${CMP_HASH}" ]; then
		echo "File ${LINE} is a copy of ${FILE}"
		FLAG=true
	fi
done < <(find "${DIR}" -type f -size "${INPUT_SIZE}c" ! -samefile "${FILE}" 2>/dev/null)

if [ "${FLAG}" = false ]; then
	echo "No copies were found"
	exit 1
fi

exit 0
