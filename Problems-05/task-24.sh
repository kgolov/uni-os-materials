#!/bin/bash

A=0
B=100

if [ $# -eq 1 ] || [ $# -gt 2 ]; then
	echo "Please run with zero or 2 parameters"
	exit 1
elif [ $# -eq 2 ]; then
	if !(echo "${1}${2}" | egrep -q "^[0-9]+$"); then
		echo "Please enter only numbers!"
		exit 2
	fi

	if [ ${1} -lt ${2} ]; then
		A="${1}"
		B="${2}"
	else
		A="${2}"
		B="${1}"
	fi
fi

GUESS=$(( (RANDOM % ${B}) + ${A} ))
TRIES=1

read -p "Guess? " NUM

while [ ${NUM} -ne ${GUESS} ]; do
	if [ ${GUESS} -lt ${NUM} ]; then
		echo "...smaller!"
	else
		echo "...bigger!"
	fi

	TRIES=$((TRIES+1))
	read -p "Guess? " NUM
done

echo "RIGHT! Guessed ${GUESS} in ${TRIES} tries!"
exit 0
