#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You need a SOURCE and DESTINATION folder"
	exit 1
fi

SOURCE="${1}"
DEST="${2}"

if [ ! -d "${SOURCE}" ] || [ ! -d "${DEST}" ]; then
	echo "Supplied parameters are not folders"
	exit 2
fi

EXTENSIONS="$(find "${SOURCE}" -type f 2>/dev/null -printf "%f\n" | awk -F '.' '{ print $2 }' | sort | uniq)"

while read EXT; do
	if [ -z "${EXT}" ]; then
		continue
	fi

	mkdir "${DEST}/${EXT}"
	
	# Can also be done with -exec mv ...
	find "${SOURCE}" -type f 2>/dev/null -name "*.${EXT}" -exec cp {} "${DEST}/${EXT}" \;
done < <(echo "${EXTENSIONS}")

exit 0
