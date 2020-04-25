#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Script needs two parameters"
	exit 1
fi

if [ ! -r "${1}" ] || [ ! -r "${2}" ]; then
	echo "One or more of the files is unreadable"
	exit 2
fi

FIRST_BASENAME="$(basename "${1}")"
SECOND_BASENAME="$(basename "${2}")"

FIRST_CNT=$(egrep -c -a "${FIRST_BASENAME}" "${1}")
SECOND_CNT=$(egrep -c -a "${SECOND_BASENAME}" "${2}")

CHOSEN=""
if [ ${FIRST_CNT} -ge ${SECOND_CNT} ]; then
	CHOSEN="${1}"
else
	CHOSEN="${2}"
fi

egrep -o -a "\".*$" "${CHOSEN}" | sort
