#!/bin/bash

if [ -z "${BACKUP_DIR}" ]; then
	echo 'You must supply a backup directory in $BACKUP_DIR'
	exit 1
fi

if [ ! -d "${BACKUP_DIR}" ]; then
	mkdir -p ${BACKUP_DIR}
fi

REC=""
DATE="$(date +"%F-%H-%M-%S")"

while [ -n "${1}" ]; do
	CURR="${1}"
	
	if [ "${CURR}" = "-r" ]; then
		REC=1
	elif [ -f "${CURR}" ]; then
		tar -cf "${BACKUP_DIR}/$(basename "${CURR}")_${DATE}.gz" "${CURR}"

		rm "${CURR}"
	elif [ -d "${CURR}" ]; then
		if [ -z "$(ls -A "${CURR}")" ]; then
			# Directory is empty
			
			tar -cf "${BACKUP_DIR}/$(basename "${CURR}")_${DATE}.tgz" "${CURR}"

			rmdir "${CURR}"
		else
			# Directory is not empty, check for recursive flag
			if [ -n "${REC}" ]; then
				tar -cf "${BACKUP_DIR}/$(basename "${CURR}")_${DATE}.tgz" "${CURR}"

				rm -r "${CURR}"
			else
				echo "error: ${CURR} is not empty, will not delete"
			fi
		fi
	fi

	shift 1
done

exit 0
