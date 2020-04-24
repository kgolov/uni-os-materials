# 5. Скриптове

### 1. (-- 05-a-2000) Сменете вашия prompt с нещо по желание. После върнете оригиналния обратно.
```bash
OLDPS=$PS1
export PS1="My Prompt> "
export PS1=$OLDPS
```

### 2. (-- 05-a-2100) Редактирайте вашия .bash_profile файл, за да ви поздравява (или да изпълнява някаква команда по ваш избор) всеки път, когато влезете в системата.
```bash
vi ~/.bashrc

# Custom greet message
echo "Hello, $(whoami)"
```

### 3. (-- 05-a-2200) Направете си ваш псевдоним (alias) на полезна команда.
```bash
alias ll = "ls -la"
```

### 4. (-- 05-b-2000) Да се напише shell скрипт, който приканва потребителя да въведе низ (име) и изпечатва "Hello, низ".
**task-04.sh**
```bash
#!/bin/bash

read -p "Please enter your name: " NAME
echo "Hello, $NAME"
```

### 5. (-- 05-b-2800) Да се напише shell скрипт, който приема точно един параметър и проверява дали подаденият му параметър се състои само от букви и цифри.
**task-05.sh**
```bash
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
```

### 6. (-- 05-b-3100) Да се напише shell скрипт, който приканва потребителя да въведе низ - потребителско име на потребител от системата - след което извежда на стандартния изход колко активни сесии има потребителят в момента.
**task-06.sh**
```bash
#!/bin/bash

read -p "Please enter a user name: " name

if fgrep -q $name /etc/passwd; then
        sessions=$(who | fgrep -c $name)
        echo "User $name has $sessions active sessions."
else
        echo "$name is not a user of this system."
fi
```

### 7. (-- 05-b-3200) Да се напише shell скрипт, който приканва потребителя да въведе пълното име на директория и извежда на стандартния изход подходящо съобщение за броя на всички файлове и всички директории в нея.
**task-07.sh**
```bash
#!/bin/bash

read -p "Enter full path to directory: " dirname

if [ ! -d $dirname ]; then
        echo "$dirname is not a directory!"
        exit 1
else
        files=$(find $dirname -type f 2> /dev/null | wc -l)
        echo "Number of files: $files"
        dirs=$(find $dirname -type d 2> /dev/null | wc -l)
        echo "Number of dirs: $dirs"
fi
```

### 8. (-- 05-b-3300) Да се напише shell скрипт, който чете от стандартния вход имената на 3 файла, обединява редовете на първите два (man paste), подрежда ги по азбучен ред и резултата записва в третия файл.
**task-08.sh**
```bash
#!/bin/bash

echo "Please enter the names of 3 files"
read -p "File one: " input1
read -p "File two: " input2
read -p "Output file: " output

if [ ! -r $input1 ] || [ ! -r $input2 ]; then
        echo "Files do not exist or read permissions are denied!"
        exit 1
fi

paste $input1 $input2 | sort -o $output

echo "Paste completed successfully in file: $output"
```

### 9. (-- 05-b-3400) Да се напише shell скрипт, който чете от стандартния вход име на файл и символен низ, проверява дали низа се съдържа във файла и извежда на стандартния изход кода на завършване на командата с която сте проверили наличието на низа.
**NB! Символният низ може да съдържа интервал (' ') в себе си.**

**task-09.sh**
```bash
#!/bin/bash

read -p "Enter full path to file: " FILENAME
read -p "Enter phrase to search for: " NEEDLE

grep -qsF "${NEEDLE}" "${FILENAME}"
echo $?
```

### 10. (-- 05-b-4200) Имате компилируем (a.k.a няма синтактични грешки) source file на езика C. Напишете shell script, който да покaзва колко е дълбоко най-дълбокото nest-ване (влагане).

**task-10.sh**
```bash
#!/bin/bash

if [ $# -ne 1 ]; then
        exit 1
fi

if [ ! -r $1 ]; then
        exit 2
fi

OPENING=0
MAX_NESTING=0

while read -n 1 CHAR; do
        if [ ${CHAR} = '{' ]; then
                OPENING=$((OPENING+1))
                if [ ${OPENING} -gt ${MAX_NESTING} ]; then
                        MAX_NESTING=${OPENING}
                fi
        else
                OPENING=$((OPENING-1))
        fi
done < <(cat $1 | sed -e 's/[^{}]//g' | tr '\n' ' ' | tr -d ' ')

echo "The deepest nesting is ${MAX_NESTING} levels"
exit 0
```

### 11. (-- 05-b-4300) Напишете shell script, който по подаден като аргумент файл с map между <nickname> -> <реален username> и nickname, ще ви улесни да изпращате съобщения на хората.

*Пример за файл указател:*
```
tinko s61966
minko s881234
ginko s62000
dinko s77777
```

Примерно извикване на програмата:
```
./send_message myAddressBook dinko
```

И вече може да изпращате съобщения на човека с username s77777.

**NB! Можете да създавате нови потребители използвайки 'sudo useradd username'. За да проверите дали се пращат съобщенията отворете 2 сесии към виртуалката ви, създайте новият потребител, логнете се с него ( 'sudo su username' от едната сесия ) и от другата сесия пратете съобщението.**

**task-11.sh**
```bash
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

if [ -z "${USER}" ]; then
	echo "There is no such user in the address book"
	exit 3
fi

# Sending message
echo "Hello, ${USER}!" | write "${USER}"
exit 0
```

### 12. (-- 05-b-4301) Напишете shell script, който автоматично да попълва файла указател от предната задача по подадени аргументи: име на файла указател, пълно име на човека (това, което очакваме да е в /etc/passwd) и избран за него nickname.

**Файлът указател нека да е във формат:**

*<nickname, който лесно да запомните> <username в os-server>
// може да сложите и друг delimiter вместо интервал*

**Примерно извикване:**
```bash
./pupulate_address_book myAddressBook "Ben Dover" uncleBen
```

**Добавя към myAddressBook entry-то:**

*uncleBen <username на Ben Dover в os-server>*

***Бонус:*** *Ако има няколко съвпадения за въведеното име (напр. има 10 човека Ivan Petrov в /etc/passwd), всички те да се показват на потребителя, заедно с пореден номер >=1,
след което той да може да въведе някой от номерата (или 0 ако не си хареса никого), и само избраният да бъде добавен към указателя.*

**task-12.sh**
```bash
#!/bin/bash

PASSWD="passwd.txt"

if [ $# -ne 3 ]; then
	echo "You must supply the address book, a full name and a nickname"
	exit 1
fi

BOOK="${1}"
NAME="${2}"
NICK="${3}"

if [ ! -r "${BOOK}" ]; then
	echo "The address book is not readable!"
	exit 2
fi

USERS="$(egrep -a "^([^:]*:){4}${NAME}(:|,)" ${PASSWD})"
COUNT="$(echo "${USERS}" | wc -l)"

if [ ${COUNT} -eq 1 ]; then
	CHOSEN_USERNAME="$(echo "${USERS}" | cut -d ':' -f 1)"
else
	echo "Please choose which user to add: "
	echo -e "N: \t USERNAME \t FULL NAME"

	NUMBER=0
	while read -r USER; do
		NUMBER=$((NUMBER+1))

		USERNAME="$(echo "${USER}" | cut -d ':' -f 1)"
		FULLNAME="$(echo "${USER}" | cut -d ':' -f 5)"
		echo -e "${NUMBER}: \t ${USERNAME} \t ${FULLNAME}"

	done < <(echo "${USERS}")

	read -p "Please input a number: " CHOSEN

	if [ ${CHOSEN} -lt 1 ] || [ ${CHOSEN} -gt ${COUNT} ]; then
		echo "Invalid number. Exiting."
		exit 3
	fi

	CHOSEN_USERNAME=$(echo "${USERS}" | sed -n "${CHOSEN} p" | cut -d ':' -f 1)
fi

echo "${CHOSEN_USERNAME} ${NICK}" >> myAddressBook
echo "Added ${NAME} (username ${CHOSEN_USERNAME}) with nickname ${NICK} to address book."

exit 0
```

### 13. (-- 05-b-4400) Напишете shell script, който да приема параметър име на директория, от която взимаме файлове, и опционално експлицитно име на директория, в която ще копираме файлове. Скриптът да копира файловете със съдържание, променено преди по-малко от 45 мин, от първата директория във втората директория. Ако втората директория не е подадена по име, нека да получи такова от днешната дата във формат, който ви е удобен. При желание новосъздадената директория да се архивира.

*TODO: архивиране по желание*

**task-13.sh**
```bash
#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "You must pass only two arguments"       
	exit 1
fi

INPUT="${1}"
if [ ! -d "${INPUT}" ]; then
	echo "The first parameter is not a directory"
	exit 2
fi

if [ $# -eq 2 ]; then
	OUTPUT="${2}"
	if [ ! -d "${OUTPUT}" ]; then
		echo "The second parameter is not a directory"
		exit 3
	fi
else
	OUTPUT="$(date +"%d-%m-%Y")"
	mkdir "${OUTPUT}"
fi

find "${INPUT}" -type f -mmin -45 2>/dev/null -exec cp {} "${OUTPUT}/" \;

exit 0
```

### 14. (-- 05-b-4500) Да се напише shell скрипт, който получава при стартиране като параметър в командния ред идентификатор на потребител. Скриптът периодично (sleep(1)) да проверява дали потребителят е log-нат, и ако да - да прекратява изпълнението си, извеждайки на стандартния изход подходящо съобщение.

**NB! Можете да тествате по същият начин като в 05-b-4300.txt**

**task-14.sh**
```bash
#!/bin/bash

PASSWD="/etc/passwd"

if [ $# -ne 1 ]; then
	echo "You must supply a User ID"
	exit 1
fi

USERNAME=$(egrep -a "^([^:]*:){2}${1}:" ${PASSWD} | cut -d ':' -f 1)

if [ -z "${USERNAME}" ]; then
	echo "There is no user with the supplied UID"
	exit 2
fi

while sleep 1; do
	COUNT=$(who | grep -c "^${USERNAME}")

	if [ ${COUNT} -gt 0 ]; then
		break
	fi
done

echo "User ${USERNAME} is logged in"
exit 0
```

### 15. (-- 05-b-4600) Да се напише shell скрипт, който валидира дали дадено цяло число попада в целочислен интервал. Скриптът приема 3 аргумента: числото, което трябва да се провери; лява граница на интервала; дясна граница на интервала. Скриптът да връща exit status:
* **3**, когато поне един от трите аргумента не е цяло число
* **2**, когато границите на интервала са обърнати
* **1**, когато числото не попада в интервала
* **0**, когато числото попада в интервала

**task-15.sh**
```bash
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
```

### 16. (-- 05-b-4700) Да се напише shell скрипт, който форматира големи числа, за да са по-лесни за четене. Като пръв аргумент на скрипта се подава цяло число. Като втори незадължителен аргумент се подава разделител. По подразбиране цифрите се разделят с празен интервал.

Примери:
```bash
$ ./nicenumber.sh 1889734853
1 889 734 853

$ ./nicenumber.sh 7632223 ,
7,632,223
```

**task-16.sh**
```bash
#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	echo "Invalid number of parameters"
	exit 1
fi

NUMBER="${1}"
DELIM=" "
if [ $# -eq 2 ]; then
	DELIM="${2}"
fi

echo ${NUMBER} | rev | sed -E "s/([0-9]{3})/\1${DELIM}/g" | rev \
	| sed -E "s/^${DELIM}//" | sed -E "s/^-${DELIM}/-/"
exit 0
```

### 17. (-- 05-b-4800) Да се напише shell скрипт, който приема файл и директория. Скриптът проверява в подадената директория и нейните под-директории дали съществува копие на подадения файл и отпечатва имената на намерените копия, ако съществуват такива.

**NB! Под 'копие' разбираме файл със същото съдържание.**

**task-17.sh**
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
	echo "A file and directory are required"
	exit 2
fi

FILE="${1}"
DIR="${2}"

if [ ! -r "${FILE}" ]; then
	echo "${FILE} is not readable"
	exit 3
fi

if [ ! -d "${DIR}" ]; then
	echo "${DIR} is not a directory"
	exit 4
fi

# Calculate input file size (in bytes) and hash, to compare later
INPUT_HASH=$(sha256sum "${FILE}" | cut -d ' ' -f 1)
INPUT_SIZE=$(stat "${FILE}" -c "%s")

# Flag to store if we have found any copies
FLAG=false

# We will search by firstly matching file size, so as to calculate hash for a smaller number of files
# We also want to find files, which are different from the input file itself
while read LINE; do
	CMP_HASH="$(sha256sum "${LINE}" | cut -d ' ' -f 1)"

	if [ "${INPUT_HASH}" = "${CMP_HASH}" ]; then
		echo "File ${LINE} is a copy of ${FILE}"
		FLAG=true
	fi
done < <(find "${DIR}" -type f -size "${INPUT_SIZE}c" ! -samefile "${FILE}" 2>/dev/null)

if [ "${FLAG}" = false ]; then
	echo "No copies were found"
	exit 1
fi

exit 0
```

### 18. (-- 05-b-5500) Да се напише shell script, който генерира HTML таблица съдържаща описание на потребителите във виртуалката ви.

**Таблицата трябва да има:**
* заглавен ред с имената нa колоните
* колони за username, group, login shell, GECKO field (man 5 passwd)

**Пример:**
```bash
$ ./passwd-to-html.sh > table.html
$ cat table.html
```
```HTML
<table>
  <tr>
    <th>Username</th>
    <th>group</th>
    <th>login shell</th>
    <th>GECKO</th>
  </tr>
  <tr>
    <td>root</td>
    <td>root</td>
    <td>/bin/bash</td>
    <td>GECKO here</td>
  </tr>
  <tr>
    <td>ubuntu</td>
    <td>ubuntu</td>
    <td>/bin/dash</td>
    <td>GECKO 2</td>
  </tr>
</table>
```

**task-18.sh**
```bash
#!/bin/bash

echo "<table>"
echo -e "\t<tr>"
echo -e "\t\t<th>Username</th>"
echo -e "\t\t<th>Group</th>"
echo -e "\t\t<th>Login shell</th>"
echo -e "\t\t<th>GECKO</th>"
echo -e "\t</tr>"

while read USER GROUP GECKO LOGSH; do
	echo -e "\t<tr>"
	echo -e "\t\t<td>${USER}</td>"
	echo -e "\t\t<td>${GROUP}</td>"
	echo -e "\t\t<td>${LOGSH}</td>"
	echo -e "\t\t<td>${GECKO}</td>"
	echo -e "\t</tr>"
done < <(cut -d ':' -f 1,4,5,7 /etc/passwd | sed 's/:/ /g')

echo "</table>"
exit 0
```

### 19. (-- 05-b-6600) Да се напише shell скрипт, който получава единствен аргумент директория и изтрива всички повтарящи се (по съдържание) файлове в дадената директория. Когато има няколко еднакви файла, да се остави само този, чието име е лексикографски преди имената на останалите дублирани файлове.

**Примери:**
```bash
$ ls .
f1 f2 f3 asdf asdf2
# asdf и asdf2 са еднакви по съдържание, но f1, f2, f3 са уникални

$ ./rmdup .
$ ls .
f1 f2 f3 asdf
# asdf2 е изтрит
```

**task-19.sh**
```bash
#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must specify a directory"
	exit 1
fi

DIR="${1}"

if [ ! -d "${DIR}" ]; then
	echo "${DIR} is not a directory"
	exit 2
fi

TODELETE=""

while read FILE; do
	# We want to find copies of this file, but not itself
	# To optimise find, we will search for files the same size
	SIZE=$(stat ${FILE} -c "%s")

	while read FOUND; do
		# Compare the found files
		if cmp -s "${FILE}" "${FOUND}"; then
			# Check lexicographical order

			if [ "${FILE}" \> "${FOUND}" ]; then
				TMP="${FOUND}"
				FOUND="${FILE}"
				FILE="${TMP}"
			fi

			# Delete the second file
			# However, mark it for deletion later
			TODELETE+=":${FOUND}"
		fi
	done < <(find ${DIR} -maxdepth 1 -type f -size "${SIZE}c" ! -samefile ${FILE})
done < <(find ${DIR} -maxdepth 1 -type f)

TODELETE=$(echo "${TODELETE}" | cut -c 2- | sed 's/:/\n/g' | sort | uniq)

if [ -z "${TODELETE}" ]; then exit 0; fi

while read FILE; do
	rm "${FILE}"
done < <(echo "${TODELETE}")
```

### 20. (-- 05-b-6800) Да се напише shell скрипт, който получава единствен аргумент директория и отпечатва списък с всички файлове и директории в нея (без скритите). До името на всеки файл да седи размера му в байтове, а до името на всяка директория да седи броят на елементите в нея (общ брой на файловете и директориите, без скритите).

**Бонус: Добавете параметър -a, който указва на скрипта да проверява и скритите файлове и директории.**

Пример:
```bash
$ ./list.sh .
asdf.txt (250 bytes)
Documents (15 entries)
empty (0 entries)
junk (1 entry)
karh-pishtov.txt (8995979 bytes)
scripts (10 entries)
```

**task-20.sh**
```bash
#!/bin/bash

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Invalid arguments - you must supply a folder"
	exit 1
fi

ALL=""
DIRNAME=""
while [ -n "${1}" ]; do
	if [ "${1}" = "-a" ] && [ -z "${ALL}" ]; then
		ALL=1
	elif [ -d "${1}" ] && [ -z "${DIRNAME}" ]; then
		DIRNAME="${1}"
	else
		echo "Invalid arguments"
		exit 1
	fi

	shift 1
done

# Firstly find directories
while read DIR; do
	# If the directory is hidden, skip it if we have not received a "-a" flag
	if (basename "${DIR}" | egrep -q "^\.") && [ -z "${ALL}" ]; then
		continue
	fi

	COUNT=""
	# Find all files and directories in that directory
	if [ -z "${ALL}" ]; then
		COUNT=$(find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null -printf "%f\n" | egrep "^[^\.]+" | wc -l)
	else
		COUNT=$(find "${DIR}" -mindepth 1 -maxdepth 1 2>/dev/null -printf "%f\n" | egrep -v "^\.$" | wc -l)
	fi

	echo "$(basename "${DIR}") (${COUNT} entries)"
done < <(find "${DIRNAME}" -mindepth 1 -maxdepth 1 2>/dev/null -type d)

# Then, read files
while read FILE; do
	# If the file is hidden, skip it if we have no "-a" flag
	if (basename "${FILE}" | egrep -q "^.") && [ -z "${ALL}" ]; then
		continue
	fi

	SIZE=$(stat ${FILE} -c "%s")

	echo "$(basename "${FILE}") (${SIZE} bytes)"
done < <(find "${DIRNAME}" -mindepth 1 -maxdepth 1 2>/dev/null -type f)

exit 0
```

### 21. (-- 05-b-7000) Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове. Скриптът да прочита от стандартния вход символен низ и за всеки от зададените файлове извежда по подходящ начин на стандартния изход броя на редовете, които съдържат низа.

**NB! Низът може да съдържа интервал.**

**task-21.sh**
```bash
#!/bin/bash

if [ $# -eq 0 ]; then
	echo "You must supply some files to search in"
	exit 1
fi

read -p "Please enter a string to search for: " NEEDLE

echo -e "Searching for the string: "\"${NEEDLE}\""\n"
while [ -n "${1}" ]; do
	FILE="${1}"

	if [ ! -r "${FILE}" ]; then
		echo "${FILE} is not readable"
		continue
	fi

	COUNT=$(egrep -c -a "${NEEDLE}" "${FILE}")
	echo -e "${FILE}\tcontains:\t${COUNT} matches"

	shift 1
done
```

### 22. (-- 05-b-7100) Да се напише shell скрипт, който приема два параметъра - име на директория и число. Скриптът да извежда на стандартния изход имената на всички обикновени файлове във директорията, които имат размер, по-голям от подаденото число.

**task-22.sh**
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
	echo "You must pass 2 parameters - a directory and a number!"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "${1} is not a directory!"
	exit 2
fi

if ! (echo "${2}" | egrep -q "^[0-9]+$"); then
	echo "${2} is not a valid number!"
	exit 3
fi

DIRNAME="${1}"
NUMBER="${2}"

OUTPUT="$(find "${DIRNAME}" -mindepth 1 -maxdepth 1 -type f -size +${NUMBER}c -printf "%f\n")"

if [ -z "${OUTPUT}" ]; then
	echo "No files found larger than ${NUMBER} bytes."
else
	echo "Files larger than ${NUMBER} bytes:"
	echo "${OUTPUT}"
fi

exit 0
```

### 23. (-- 05-b-7200) Да се напише shell скрипт, който приема произволен брой аргументи - имена на файлове или директории. Скриптът да извежда за всеки аргумент подходящо съобщение:
* дали е файл, който може да прочетем
* ако е директория - имената на файловете в нея, които имат размер, по-малък от броя на файловете в директорията.

**task-23.sh**
```bash
#!/bin/bash

while [ -n "${1}" ]; do
	PARAM="${1}"
	if [ -f "${PARAM}" ]; then
		if [ -r "${PARAM}" ]; then
			echo "${PARAM} is a readable file"
		else
			echo "${PARAM} is a file, but it is not readable"
		fi
	elif [ -d "${PARAM}" ]; then
		COUNT=$(find "${PARAM}" -mindepth 1 -maxdepth 1 -type f | wc -l)
		echo "${PARAM} is a directory with ${COUNT} files. The following files are bigger than ${COUNT} bytes: "

		OUTPUT="$(find "${PARAM}" -mindepth 1 -maxdepth 1 -type f -size +${COUNT}c -printf "%f\n")"
		if [ -z "${OUTPUT}" ]; then
			echo "No files are bigger than ${COUNT} bytes"
		else
			echo "${OUTPUT}"
		fi
	fi

	shift 1
done
```

### 24. (-- 05-b-7500) Напишете shell script guess, която си намисля число, което вие трябва да познате. В зависимост от вашия отговор, програмата трябва да ви казва "надолу" или "нагоре", докато не познате числото. Когато го познаете, програмата да ви казва с колко опита сте успели.

```bash
./guess #(програмата си намисля 5)
```
```
Guess? 22
...smaller!
Guess? 1
...bigger!
Guess? 4
...bigger!
Guess? 6
...smaller!
Guess? 5
RIGHT! Guessed 5 in 5 tries!
```

**Hint:** Един начин да направите рандъм число е с $(( (RANDOM % b) + a  )), което ще генерира число в интервала [a, b]. Може да вземете a и b като параметри, но не забравяйте да направите проверката.

**task-24.sh**
```bash
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
```

### 25. (-- 05-b-7550) Да се напише shell скрипт, който приема параметър - име на потребител. Скриптът да прекратява изпълненито на всички текущо работещи процеси на дадения потребител, и да извежда колко са били те.

**NB! Може да тествате по същият начин като описаният в 05-b-4300**

**task-25.sh**
```bash
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
```

### 26. (-- 05-b-7700) Да се напише shell скрипт, който приема два параметъра - име на директория и число. Скриптът да извежда сумата от размерите на файловете в директорията, които имат размер, по-голям от подаденото число.

**task-26.sh**
```bash
#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Please enter a directory and a number"
	exit 1
fi

DIR="${1}"
NUM="${2}"

if [ ! -d "${DIR}" ]; then
	echo "You must supply a directory"
	exit 2
fi

if !(echo "${NUM}" | egrep -q "^[0-9]+$"); then
	echo "You must supply a valid number"
	exit 2
fi

find "${DIR}" -maxdepth 1 -type f -size "+${NUM}c" -printf "%s\n" | awk '{ sum += $1 } END { print sum }'
exit 0
```

### 27. (-- 05-b-7800) Да се напише shell скрипт, който намира броя на изпълнимите файлове в PATH.
**Hint: Предполага се, че няма спейсове в имената на директориите**
**Hint2: Ако все пак искаме да се справим с този случай, да се разгледа IFS променливата и констуркцията while read -d**

**task-27.sh**
```bash
#!/bin/bash

SUM=0

while IFS= read -d ':' DIR; do
	COUNT=$(find "${DIR}" -type f -executable -printf '.' | wc -m)

	SUM=$((SUM+COUNT))
done < <(echo "${PATH}:")

echo ${SUM}

exit 0
```

### 28. (-- 05-b-8000) Напишете shell script, който получава като единствен аргумент име на потребител и за всеки негов процес изписва съобщение за съотношението на RSS към VSZ. Съобщенията да са сортирани, като процесите с най-много заета виртуална памет са най-отгоре.

**Hint:**
Понеже в Bash няма аритметика с плаваща запетая, за смятането на съотношението използвайте командата bc. За да сметнем нампример 24/7, можем да: echo "scale=2; 24/7" | bc
Резултатът е 3.42 и има 2 знака след десетичната точка, защото scale=2.
Алтернативно, при липса на bc ползвайте awk.

**task-28.sh**
```bash
#!/bin/bash

if [ $# -ne 1 ]; then
	echo "You must supply a user"
	exit 1
fi

USR="${1}"
if !(egrep -q -a "^${USR}:" /etc/passwd); then
	echo "No such user exists"
	exit 2
fi

PS="$(ps --no-header -u ${USR} -o rss,vsz,pid | tr -s ' ' | egrep -o "([0-9]+ ){2}[0-9]+$")"
PS_SORTED="$(echo "${PS}" | sort -k 2 -n -r)"

while read RSS VSZ PID; do
	if [ ${VSZ} -ne 0 ]; then
		CALC="$(echo "scale=4; ${RSS}/${VSZ}" | bc)"
	else
		CALC="No VSZ"
	fi

	echo -e "PID: ${PID}\tRSS/VSZ: ${CALC}"
done < <(echo "${PS_SORTED}")

exit 0
```

### 29. (-- 05-b-9100) Опишете поредица от команди или напишете shell скрипт, които/който при известни две директории SOURCE и DESTINATION:
* намира уникалните "разширения" на всички файлове, намиращи се някъде под SOURCE. (За простота приемаме, че в имената на файловете може да се среща символът точка '.' максимум веднъж.)
* за всяко "разширение" създава по една поддиректория на DESTINATION със същото име
* разпределя спрямо "разширението" всички файлове от SOURCE в съответните поддиректории в DESTINATION

**task-29.sh**
```bash

```

### 30. (-- 05-b-9200) Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива. Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива. За всеки изтрит файл (директория) скриптът добавя ред във log файл с подходящо съобщение.

**а)** Името на log файла да се чете от shell environment променлива, която сте конфигурирали във вашия .bashrc.
**б)** Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно.
**в)** Добавете timestamp на log съобщенията във формата: 2018-05-01 22:51:36

**Примери:**
$ export RMLOG_FILE=~/logs/remove.log
$ ./rmlog -r f1 f2 f3 mydir/ emptydir/
$ cat $RMLOG_FILE
[2018-04-01 13:12:00] Removed file f1
[2018-04-01 13:12:00] Removed file f2
[2018-04-01 13:12:00] Removed file f3
[2018-04-01 13:12:00] Removed directory recursively mydir/
[2018-04-01 13:12:00] Removed directory emptydir/

**task-30.sh**
```bash

```

### 31. (-- 05-b-9500) (Цветно принтиране) Напишете shell script color_print, който взима два параметъра.

**Първият може да е измежду "-r", "-g" "-b", а вторият е произволен string.
На командата "echo" може да се подаде код на цвят, който ще оцвети текста в определения цвят.
В зависимост от първия аргумент, изпринтете втория аргумен в определения цвят:

"-r" е червено. Кодът на червеното е '\033[0;31m' (echo -e "\033[0;31m This is red")
"-g" е зелено. Кодът на зеленото е '\033[0;32m' (echo -e "\033[0;32m This is green")
"-b" е синьо. Кодът на синьото е '\033[0;34m' (echo -e "\033[0;34m This is blue")
Ако е подадена друга буква изпишете "Unknown colour", а ако изобщо не е подаден аргумент за цвят, просто изпишете текста.

Hint:**

В края на скрипта си напишете:
echo '\033[0m'
, за да не се прецакат цветовете на терминала. Това е цветът на "няма цвят".

**task-31.sh**
```bash

```

### 32. (-- 05-b-9501) Този път програмата ви ще приема само един параметър, който е измежду ("-r", "-b", "-g", "-x"). Напишете shell script, който приема редовете от stdin и ги изпринтва всеки ред с редуващ се цвят. Цветовете вървят RED-GREEN-BLUE и цветът на първия ред се определя от аргумента.  Ако е подаден аргумент "-x", то не трябва да променяте цветовете в терминала (т.е., все едно сте извикали командата cat).

**Hint:** Не забравяйте да връщате цветовете в терминала.

**task-32.sh**
```bash

```

### 33. (-- 05-b-9600) Да се напише shell скрипт, който получава произволен брой аргументи файлове, които изтрива. Ако бъде подадена празна директория, тя бива изтрита. Ако подадения файл е директория с поне 1 файл, тя не се изтрива.

Да се дефинира променлива BACKUP_DIR (или друго име), в която:
* изтритите файлове се компресират и запазват
* изтритите директории се архивират, комприсират и запазват
* имената на файловете е "filename_yyyy-mm-dd-HH-MM-SS.{gz,tgz}", където filename е оригиналното име на файла (директорията) преди да бъде изтрит

**а)** Добавете параметър -r на скрипта, който позволява да се изтриват непразни директории рекурсивно и съответно да се запазят в BACKUP_DIR

**Примери:**
```bash
$ export BACKUP_DIR=~/.backup/

# full-dir/ има файлове и не може да бъде изтрита без параметър -r
$ ./trash f1 f2 full-dir/ empty-dir/
error: full-dir/ is not empty, will not detele
$ ls $BACKUP_DIR
f1_2018-05-07-18-04-36.gz
f2_2018-05-07-18-04-36.gz
empty-dir_2018-05-07-18-04-36.tgz

$ ./trash -r full-dir/

$ ls $BACKUP_DIR
f1_2018-05-07-18-04-36.gz
f2_2018-05-07-18-04-36.gz
full-dir_2018-05-07-18-04-50.tgz
empty-dir_2018-05-07-18-04-36.tgz

# можем да имаме няколко изтрити файла, които се казват по един и същ начин
$ ./trash somedir/f1

$ ls $BACKUP_DIR
f1_2018-05-07-18-04-36.gz
f1_2018-05-07-18-06-01.gz
f2_2018-05-07-18-04-36.gz
full-dir_2018-05-07-18-04-50.tgz
empty-dir_2018-05-07-18-04-36.tgz
```

**task-33.sh**
```bash

```

### 34. (-- 05-b-9601) Да се напише shell скрипт, който възстановява изтрити файлове, които имат запазено копие в BACKUP_DIR (от предната задача). При възстановяването файловете да се декомпресират, а директориите да се декомпресират и разархивират.

**а)** Да се дефинира параметър -l, който изрежда всички файлове, които могат да бъдат възстановени и датата на тяхното изтриване.

**б)** Скриптът да приема 2 параметъра. Първият е името на файла, който да се възстанови, а вторията е директорията, в която файлът да бъде възстановен. Ако вторият аргумент липсва, файлът да се възстановява в сегашната директория, където скриптът се изпълнява.

**в)** Когато има N > 1 запазени файла със същото име, да се изпише списък с N реда на потребителя и да се изиска той да въведе цяло число от 1 до N, за да избере кой файл да възстанови.

**Примери:**
```bash
# BACKUP_DIR трябва да е дефинирана преди използването на скрипта
$ echo $BACKUP_DIR
~/.backup

$ ./restore.sh -l
f1  (2018/05/07 18:04:36)
f1  (2018/05/07 18:06:01)
f2  (2018/05/07 18:04:36)
full-dir    (2018/05/07 18:04:50)
empty-dir   (2018/05/07 18:04:36)

$ ls restored-dir/
# възстановяване на файл в подадена директория
$ ./restore.sh f2 target-dir/
$ ls restored-dir/
f2

# възстановяване на дублиран файл в сегашната директория
$ ./restore.sh f1
(1) f1  (2018/05/07 18:04:36)
(2) f1  (2018/05/07 18:06:01)
choose file (1, 2):
# потребителят въвежда 2

$ ls
f1

$ ./restore.sh -l
f1  (2018/05/07 18:04:36)
full-dir    (2018/05/07 18:04:50)
empty-dir   (2018/05/07 18:04:36)

# възстановяване на директория в сегашната директория
$ ./restore.sh full-dir
$ ls
f1  full-dir/
```

**task-34.sh**
```bash

```
