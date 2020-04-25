#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You need to pass two numbers as parameters"
	exit 1
fi

if !(echo "${1}${2}" | egrep -q "^[0-9]+$"); then
	echo "Only numbers are supported!"
	exit 2
fi

LWR="${1}"
UPR="${2}"

if [ ${LWR} -gt ${UPR} ]; then 
	TMP="${LWR}"
	LWR="${UPR}"
	UPR="${TMP}"
fi

mkdir a b c

while read FILE; do
	MOVE_TO=""
	LINES="$(cat "${FILE}" | wc -l)"
	
	if [ ${LINES} -lt ${LWR} ]; then
		MOVE_TO="a"
	elif [ ${LINES} -gt ${UPR} ]; then
		MOVE_TO="c"
	else
		MOVE_TO="b"
	fi

	mv "${FILE}" "${MOVE_TO}/"
done < <(find . -type f ! -samefile "${0}")
