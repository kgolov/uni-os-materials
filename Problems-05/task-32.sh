#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must supply a color only!"
	exit 1
fi

COLOR="${1}"

while IFS= read LINE; do
	case "${COLOR}" in
        	"-r")
                	echo -e "\033[0;31m${LINE}"
									COLOR="-g"
        	;;
        	"-g")
                	echo -e "\033[0;32m${LINE}"
									COLOR="-b"
        	;;
        	"-b")
                	echo -e "\033[0;34m${LINE}"
									COLOR="-r"
        	;;
        	"-x")
                	echo "${LINE}"
        	;;
	esac
done

echo -n -e "\033[0m"
exit 0
