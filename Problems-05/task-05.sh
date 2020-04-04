#!/bin/bash

if [ $# -ne 1 ]; then
	echo "We don't have exactly one parameter."
	exit 1
fi

if grep -q "^[[:alnum:]]*$" <(echo $1); then
	echo "The parameter is alphanumeric."
else 
	echo "The parameter is NOT alphanumeric."
fi
