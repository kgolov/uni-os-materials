#!/bin/bash

# Print output of ps command to file to work with
ps -eo pid,ppid | tail -n +2 | tr -s ' ' | egrep -o "[0-9]+ [0-9]+" > ps_output.txt

while read pid ppid; do
	if [ ! -f "kids_${pid}" ]; then
		numkids=$(egrep -c "^[0-9]+ ${pid}$" ps_output.txt)
		echo ${numkids} > "kids_${pid}"
		echo "${pid}" >> processes.txt
	fi

	if [ ! -f "kids_${ppid}" ]; then
		numkids=$(egrep -c "^[0-9]+ ${ppid}$" ps_output.txt)
		echo ${numkids} > "kids_${ppid}"
		echo "${ppid}" >> processes.txt
	fi
done < ps_output.txt

sort -n processes.txt | uniq > processes_sorted.txt

while read pid; do
	if [ -f "kids_${pid}" ]; then
		this_kids="$(cat kids_${pid})"
	else 
		this_kids=0
	fi

	ppid="$(egrep -a "^${pid} " ps_output.txt | cut -d ' ' -f 2)"

	if [ -n "${ppid}" ]; then
		parent_kids="$(cat kids_${ppid})"

		if [ ${this_kids} -gt ${parent_kids} ]; then
			echo "The process with PID ${pid} has more children than its parent process ${ppid}"
			echo "Number of kids of process: ${this_kids}"
			echo "Number of kids of parent: ${parent_kids}"
		fi
	fi
done < processes_sorted.txt

rm kids_*
rm ps_output.txt
rm processes.txt
rm processes_sorted.txt

exit 0
