#!/bin/bash

PASSWD="/etc/passwd"

if [ $# -ne 1 ]; then
	echo "You must supply a User ID"
	exit 1
fi

USERNAME=$(egrep -a "^([^:]*:){2}${1}:" ${PASSWD} | cut -d ':' -f 1) 

if [ -z "${USERNAME}" ]; then
	echo "There is no user with the supplied UID"
	exit 2
fi

while sleep 1; do
	COUNT=$(who | grep -c "^${USERNAME}")

	if [ ${COUNT} -gt 0 ]; then
		break
	fi
done

echo "User ${USERNAME} is logged in"
exit 0
