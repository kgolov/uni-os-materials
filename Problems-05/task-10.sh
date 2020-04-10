#!/bin/bash

if [ $# -ne 1 ]; then
	exit 1
fi

if [ ! -r $1 ]; then
	exit 2
fi

OPENING=0
MAX_NESTING=0

while read -n 1 CHAR; do
	if [ ${CHAR} = '{' ]; then
		OPENING=$((OPENING+1))
		if [ ${OPENING} -gt ${MAX_NESTING} ]; then
			MAX_NESTING=${OPENING}
		fi
	else
		OPENING=$((OPENING-1))
	fi
done < <(cat $1 | sed -e 's/[^{}]//g' | tr '\n' ' ' | tr -d ' ')

echo "The deepest nesting is ${MAX_NESTING} levels"
exit 0
