#!/bin/bash

if [ $# -ne 3 ]; then
	echo "You must supply 3 arguments"
	exit 4
fi

if (! echo $1 | grep -q "^[0-9]*$") || (! echo $2 | grep -q "^[0-9]*$") || (! echo $3 | grep -q "^[0-9]$"); then
	echo "One of your numbers is not an integer"
	exit 3
fi

if [ $2 -gt $3 ]; then
	echo "The interval start and end are flipped"
	exit 2
fi

if [ $1 -lt $2 ] || [ $1 -gt $3 ]; then
	echo "The number is not in the interval"
	exit 1
fi

echo "The number is in the interval"
exit 0
