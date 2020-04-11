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

USERS="$(egrep -a "^([^:]*:){4}${NAME}(:|,)" ${PASSWD})"
COUNT="$(echo "${USERS}" | wc -l)"

if [ ${COUNT} -eq 1 ]; then
	CHOSEN_USERNAME="$(echo "${USERS}" | cut -d ':' -f 1)"
else
	echo "Please choose which user to add: "
	echo -e "N: \t USERNAME \t FULL NAME"

	NUMBER=0
	while read -r USER; do
		NUMBER=$((NUMBER+1))

		USERNAME="$(echo "${USER}" | cut -d ':' -f 1)"
		FULLNAME="$(echo "${USER}" | cut -d ':' -f 5)"
		echo -e "${NUMBER}: \t ${USERNAME} \t ${FULLNAME}"
		
	done < <(echo "${USERS}")

	read -p "Please input a number: " CHOSEN

	if [ ${CHOSEN} -lt 1 ] || [ ${CHOSEN} -gt ${COUNT} ]; then
		echo "Invalid number. Exiting."
		exit 3
	fi

	CHOSEN_USERNAME=$(echo "${USERS}" | sed -n "${CHOSEN} p" | cut -d ':' -f 1)
fi

echo "${CHOSEN_USERNAME} ${NICK}" >> myAddressBook
echo "Added ${NAME} (username ${CHOSEN_USERNAME}) with nickname ${NICK} to address book."

exit 0
