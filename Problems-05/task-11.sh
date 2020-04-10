#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must supply an address book and a username"
	exit 1
fi

if [ ! -r $1 ]; then
	echo "Address book is not readable"
	exit 2
fi

USER=$(fgrep "$2" $1 | cut -d ' ' -f 2)

# Sending message?
#write $USER
#echo "hello"
