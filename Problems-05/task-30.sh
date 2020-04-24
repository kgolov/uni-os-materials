#!/bin/bash

if [ -z "${RMLOG_FILE}" ]; then
	echo 'You must set log file location with the environment variable $RMLOG_FILE'
	exit 1
fi

if [ -f "${RMLOG_FILE}" ] && [ ! -w "${RMLOG_FILE}" ]; then
	echo '$RMLOG_FILE must be writable'
	exit 2
elif [ ! -f "${RMLOG_FILE}" ]; then
	touch "${RMLOG_FILE}"
fi

REC=""
DATE="$(date +"[%F %H:%M:%S]")"

while [ -n "${1}" ]; do
	CURR="${1}"
	
	if [ "${CURR}" = "-r" ]; then
		REC=1
	elif [ -f "${CURR}" ]; then
		rm "${CURR}"
		
		echo "${DATE} Removed file ${CURR}" >> "${RMLOG_FILE}"
	elif [ -d "${CURR}" ]; then
		if [ -z "$(ls -A "${CURR}")" ]; then
			# Directory is empty

			rmdir "${CURR}"

			echo "${DATE} Removed directory ${CURR}" >> "${RMLOG_FILE}"
		else
			# Directory is not empty, check for recursive flag
			if [ -n "${REC}" ]; then
				rm -r "${CURR}"

				echo "${DATE} Removed recursively directory ${CURR}" >> "${RMLOG_FILE}"
			fi
		fi
	fi

	shift 1
done

exit 0
