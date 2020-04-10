#!/bin/bash

PASSWD="passwd.txt"

if [ $# -ne 3 ]; then
	echo "You must supply the address book, a full name and a nickname"
	exit 1
fi

BOOK="${1}"
NAME="${2}"
NICK="${3}"

if [ ! -r "${BOOK}" ]; then
	echo "The address book is not readable!"
	exit 2
fi

USER="$(egrep -a "^([^:]*:){4}${NAME}(:|,)" ${PASSWD} | cut -d ':' -f 1)"

if [ -z "${USER}" ]; then
	echo "User with such name is not found"
	exit 3
fi 

echo "${NICK} ${USER}" >> myAddressBook
exit 0
