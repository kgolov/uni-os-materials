#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must supply a user"
	exit 1
fi

USR="${1}"
if !(egrep -q -a "^${USR}:" /etc/passwd); then
	echo "No such user exists"
	exit 2
fi

PS="$(ps --no-header -u ${USR} -o rss,vsz,pid | tr -s ' ' | egrep -o "([0-9]+ ){2}[0-9]+$")"
PS_SORTED="$(echo "${PS}" | sort -k 2 -n -r)"

while read RSS VSZ PID; do
	if [ ${VSZ} -ne 0 ]; then
		CALC="$(echo "scale=4; ${RSS}/${VSZ}" | bc)"
	else 
		CALC="No VSZ"
	fi

	echo -e "PID: ${PID}\tRSS/VSZ: ${CALC}"
done < <(echo "${PS_SORTED}")

exit 0
