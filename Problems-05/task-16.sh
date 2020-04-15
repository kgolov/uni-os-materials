#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "Invalid number of parameters"
	exit 1
fi

NUMBER="${1}"
DELIM=" "
if [ $# -eq 2 ]; then
	DELIM="${2}"
fi

echo ${NUMBER} | rev | sed -E "s/([0-9]{3})/\1${DELIM}/g" | rev \
	| sed -E "s/^${DELIM}//" | sed -E "s/^-${DELIM}/-/"
exit 0
