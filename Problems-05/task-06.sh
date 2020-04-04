#!/bin/bash

read -p "Please enter a user name: " name

if fgrep -q $name /etc/passwd; then
	sessions=$(who | fgrep -c $name)
	echo "User $name has $sessions active sessions."
else
	echo "$name is not a user of this system."
fi
