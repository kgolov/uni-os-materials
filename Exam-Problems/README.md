# Примерни задачи от контролно 1

## Условията на задачите се намират [тук](Exam-Problems.pdf)

### Зад. 1
```bash
egrep -c '^[^a-w]*[02468]+[^a-w]*$' philip-j-fry.txt
```

### Зад. 2
**task-02.sh**
```bash
#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Need file with books"
	exit 1
fi

FILE="${1}"

if [ ! -r "${FILE}" ]; then
	echo "File is not readable"
	exit 2
fi

COUNTER=1
while read line; do
	echo "${COUNTER}.$(echo "${line}" | cut -d "-" -f 2-)"
	COUNTER=$((COUNTER+1))
done < "${FILE}" | sort -t '.' -k 2
```

### Зад. 3
**task-03.sh**
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You need to pass two numbers as parameters"
	exit 1
fi

if !(echo "${1}${2}" | egrep -q "^[0-9]+$"); then
	echo "Only numbers are supported!"
	exit 2
fi

LWR="${1}"
UPR="${2}"

if [ ${LWR} -gt ${UPR} ]; then
	TMP="${LWR}"
	LWR="${UPR}"
	UPR="${TMP}"
fi

mkdir a b c

while read FILE; do
	MOVE_TO=""
	LINES="$(cat "${FILE}" | wc -l)"

	if [ ${LINES} -lt ${LWR} ]; then
		MOVE_TO="a"
	elif [ ${LINES} -gt ${UPR} ]; then
		MOVE_TO="c"
	else
		MOVE_TO="b"
	fi

	mv "${FILE}" "${MOVE_TO}/"
done < <(find . -type f ! -samefile "${0}")
```

### Зад. 4
**task-04.sh**
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Script needs two parameters"
	exit 1
fi

if [ ! -r "${1}" ] || [ ! -r "${2}" ]; then
	echo "One or more of the files is unreadable"
	exit 2
fi

FIRST_BASENAME="$(basename "${1}")"
SECOND_BASENAME="$(basename "${2}")"

FIRST_CNT=$(egrep -c -a "${FIRST_BASENAME}" "${1}")
SECOND_CNT=$(egrep -c -a "${SECOND_BASENAME}" "${2}")

CHOSEN=""
if [ ${FIRST_CNT} -ge ${SECOND_CNT} ]; then
	CHOSEN="${1}"
else
	CHOSEN="${2}"
fi

egrep -o -a "\".*$" "${CHOSEN}" | sort
```

### Зад. 5
```bash
find / -user $(whoami) -printf '.' 2> /dev/null | wc -c
```

### Зад. 6а
```bash
find . -type f -size 0 -exec rm -f {} +
```

### Зад. 6б
**Внимавайте с изтриването на файлове в home директорията**
```bash
find ~ -type f -printf "%s %p\n" 2> /dev/null | sort -n -r | head -n 5 | cut -d ' ' -f 2 | xargs rm -f
```

### Зад. 7
```bash
grep -i 'inf' passwd_test | awk -F '[,:]' '$5 ~ /a$/ {print $1}' | cut -c3-4 | sort | uniq -c
```

### Зад. 8
```bash
find . -type f -printf "%n %p\n" 2> /dev/null | sort -n -r | head -n 5 | cut -d ' ' -f 2
```

### Зад. 9
```bash
find ~ -type f -printf "%T@ %n %i\n" 2> /dev/null | sort -n -r | awk '$2 > 1 { print $3 }' | head -n 1
```

### Зад. 10
```bash
egrep -a "^([^:]*:){3}$(sort -t ':' -k 3 passwd.txt | head -n 201 | tail -n 1 | cut -d ':' -f 4):" passwd.txt | sort -n | cut -d ':' -f 5,6 | sed 's/,[^,:]*:/:/'
```

### Зад. 11
*Забележка: не е тествано*
```bash
find /home/SI -maxdepth 1 -type d -newerct 1551168000 -not -newerct 1551176100 -exec grep -F :{}: /etc/passwd \; | cut -d ':' -f 1,5 | cut -c2- | cut -d ',' -f 1 | tr ':' '\t'
```

### Зад. 12а
*Забележка: търсим в home директорията на текущия потребител*
```bash
find ~ -type f 2> /dev/null -inum $(find ~ -type f -printf "%T@ %i\n" 2> /dev/null | sort -n -r | head -n 1 | cut -d ' ' -f 2) | egrep -o '.' | fgrep -c '/'
```

### Зад. 12б
```bash
find ~ -maxdepth 1 -type f | head -n 1 | grep -o '.' | fgrep -c '/'
```
