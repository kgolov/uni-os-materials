#!/bin/bash

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Script needs two parameters"
	exit 1
elif [ $# -eq 1 ]; then
	echo "${1}"
	exit 0
fi

COLOR="${1}"
TXT="${2}"

case "${COLOR}" in
	"-r")
		echo -e "\033[0;31m${TXT}"
	;;
	"-g") 
		echo -e "\033[0;32m${TXT}"
	;;
	"-b")
		echo -e "\033[0;34m${TXT}"
	;;
	*)
		echo "Unknown color"
	;;
esac

echo -n -e "\033[0m"
exit 0
