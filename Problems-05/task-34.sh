#!/bin/bash

if [ -z "${BACKUP_DIR}" ] || [ ! -d "${BACKUP_DIR}" ]; then
	echo 'You must supply a backup directory in $BACKUP_DIR'
	exit 1
fi

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Wrong number of arguments"
	exit 2
fi

function display_file {
	FNAME="$(echo "${1}" | cut -d '_' -f 1)"
	DATE="$(echo "${1}" | cut -d '_' -f 2 | cut -d '.' -f 1 |\
	       	awk -F '-' '{ print "("$1"/"$2"/"$3" "$4":"$5":"$6")" }')"
	
	echo -e "${FNAME}\t${DATE}"
}

RESTORE_IN="."

if [ $# -eq 2 ]; then
	RESTORE_IN="${2}"
fi

if [ "${1}" = "-l" ]; then
	# List files that can be restored
	while read FOUND; do
		display_file "${FOUND}"
	done < <(find "${BACKUP_DIR}" -name "*gz" -printf "%f\n" | sort -t '.' -k 2)
	exit 0
fi

TO_RESTORE="${1}"
FOUND="$(find "${BACKUP_DIR}" -name "${TO_RESTORE}*" -printf "%f\n")"

if [ -z "${FOUND}" ]; then
	echo "No such file or directory found"
	exit 3
fi

CNT="$(echo "${FOUND}" | wc -l)"

if [ ${CNT} -gt 1 ]; then
	NUM=1
	while read OPTION; do
		echo "(${NUM}) $(display_file "${OPTION}")"
		NUM=$((NUM+1))
	done < <(echo "${FOUND}")
	read -p "choose file (1-${CNT}): " CHOSEN

	if [ ${CHOSEN} -lt 1 ] || [ ${CHOSEN} -gt ${CNT} ]; then
		echo "Please enter a valid number! Exiting."
		exit 4
	fi

	TO_RESTORE="$(echo "${FOUND}" | head -n ${CHOSEN} | tail -n 1)"
else
	TO_RESTORE="${FOUND}"
fi

tar -xf "${BACKUP_DIR}/${TO_RESTORE}" -C "${RESTORE_IN}"
rm "${BACKUP_DIR}/${TO_RESTORE}"

exit 0
