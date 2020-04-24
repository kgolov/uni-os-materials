#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
	echo "Script must be run as root"
	exit 1
fi

if [ $# -ne 1 ]; then
	echo "Please enter a username"
	exit 2
fi

USR="${1}"

if !(egrep -q -a "^${USR}:" /etc/passwd); then
	echo "No such user"
	exit 3
fi

CNT="$(ps --no-header -o pid -u ${USR} | wc -l)"

killall -u "${USR}"

sleep 1

# See if processes have been terminated successfully.
TOKILL="$(ps --no-header -o pid -u ${USR} | wc -l)"

if [ ${TOKILL} -ne 0 ]; then
	# If they havent, try sending SIGKILL
	killall -s SIGKILL -u "${USR}"

	sleep 1
fi

# Count processes once again
NEWCNT="$(ps --no-header -o pid -u ${USR} | wc -l)"

if [ ${NEWCNT} -eq 0 ]; then
	echo "Killed ${CNT} processes."
else
	KILLED=$((CNT-NEWCNT))
	echo "Killed ${KILLED} processes. Couldn't kill ${NEWCNT} processes."
fi

exit 0
