# 4. Процеси

### 1. (-- 04-a-5000) Намерете командите на 10-те най-стари процеси в системата.
```bash
ps -eo etimes,cmd | tail -n +2 | sort -n -r | head | awk '{print $2}'
ps -eo cmd --sort=start_time | head -n 11
```

### 2. (-- 04-a-6000) Намерете PID и командата на процеса, който заема най-много виртуална памет в системата.
```bash
ps -eo vsz,pid,cmd | tail -n +2 | sort -n -r | head -n 1 | awk '{print $2, $3}'
ps -eo pid,cmd --sort=vsz | head -n 2
```

### 3. (-- 04-a-6300) Изведете командата на най-стария процес
```bash
ps -eo etimes,cmd | tail -n +2 | sort -n -r | head -n 1 | awk '{print $2}'
ps -eo cmd --sort=start_time | head -n 2
```

### 4. (-- 04-b-5000) Намерете колко физическа памет заемат всички процеси на потребителската група students.
```bash
ps -o rsz,cmd --group "students"
```

### 5. (-- 04-b-6100) Изведете имената на потребителите, които имат поне 2 процеса, чиято команда е vim
```bash
ps -o user -C "vim" | tail -n +2 | sort | uniq -c | awk '$1 > 1 {print $2}'
```

### 6. (-- 04-b-6200) Изведете имената на потребителите, които не са логнати в момента, но имат живи процеси
```bash
ps -eo user | sort | uniq | fgrep -v "$(who | awk '{print $1}' | tr '\n' '\|' | sed 's/.$//')"
```

### 7. (-- 04-b-7000) Намерете колко физическа памет заемат осреднено всички процеси на потребителската група students. Внимавайте, когато групата няма нито един процес.
```bash
ps -o rsz --group "students" 2> /dev/null | awk '{sum += $1} END {if (NR > 0) { print sum/NR }}'
```

### 8. (-- 04-b-8000) Намерете всички PID и техните команди (без аргументите), които нямат tty, което ги управлява. Изведете списък само с командите без повторения.
```bash
ps -eo tty,pid,cmd | grep -v "^?" | awk '{print $2, $3}'
```

### 9. (-- 04-b-9000) Да се отпечатат PID на всички процеси, които имат повече деца от родителския си процес.

**task-09.sh**
```bash
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
```
