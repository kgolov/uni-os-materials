#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must specify a directory"
	exit 1
fi

DIR="${1}"

if [ ! -d "${DIR}" ]; then
	echo "${DIR} is not a directory"
	exit 2
fi

TODELETE=""

while read FILE; do
	# We want to find copies of this file, but not itself
	# To optimise find, we will search for files the same size
	SIZE=$(stat ${FILE} -c "%s")

	while read FOUND; do
		# Compare the found files
		if cmp -s "${FILE}" "${FOUND}"; then
			# Check lexicographical order

			if [ "${FILE}" \> "${FOUND}" ]; then
				TMP="${FOUND}"
				FOUND="${FILE}"
				FILE="${TMP}"
			fi

			# Delete the second file
			# However, mark it for deletion later
			TODELETE+=":${FOUND}"
		fi
	done < <(find ${DIR} -maxdepth 1 -type f -size "${SIZE}c" ! -samefile ${FILE})
done < <(find ${DIR} -maxdepth 1 -type f)

TODELETE=$(echo "${TODELETE}" | cut -c 2- | sed 's/:/\n/g' | sort | uniq)

if [ -z "${TODELETE}" ]; then exit 0; fi

while read FILE; do
	rm "${FILE}"
done < <(echo "${TODELETE}")
