# 5. Скриптове

### 1. Сменете вашия prompt с нещо по желание. После върнете оригиналния обратно.
```bash
OLDPS=$PS1
export PS1="My Prompt> "
export PS1=$OLDPS
```

### 2. Редактирайте вашия .bash_profile файл, за да ви поздравява (или да изпълнява някаква команда по ваш избор) всеки път, когато влезете в системата.
```bash
vi ~/.bashrc

# Custom greet message
echo "Hello, $(whoami)"
```

### 3. Направете си ваш псевдоним (alias) на полезна команда.
```bash
alias ll = "ls -la"
```

### 4. Да се напише shell скрипт, който приканва потребителя да въведе низ (име) и изпечатва "Hello, низ".
**task-04.sh**
```bash
#!/bin/bash

read -p "Please enter your name: " NAME
echo "Hello, $NAME"
```

### 5. Да се напише shell скрипт, който приема точно един параметър и проверява дали подаденият му параметър се състои само от букви и цифри.
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

### 6. Да се напише shell скрипт, който приканва потребителя да въведе низ - потребителско име на потребител от системата - след което извежда на стандартния изход колко активни сесии има потребителят в момента.
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

### 7. Да се напише shell скрипт, който приканва потребителя да въведе пълното име на директория и извежда на стандартния изход подходящо съобщение за броя на всички файлове и всички директории в нея.
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

### 8. Да се напише shell скрипт, който чете от стандартния вход имената на 3 файла, обединява редовете на първите два (man paste), подрежда ги по азбучен ред и резултата записва в третия файл.
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

### 9. Да се напише shell скрипт, който чете от стандартния вход име на файл и символен низ, проверява дали низа се съдържа във файла и извежда на стандартния изход кода на завършване на командата с която сте проверили наличието на низа.
**NB! Символният низ може да съдържа интервал (' ') в себе си.**
```bash

```

### 10. Имате компилируем (a.k.a няма синтактични грешки) source file на езика C. Напишете shell script, който да покaзва колко е дълбоко най-дълбокото nest-ване (влагане).
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

### 11. Напишете shell script, който по подаден като аргумент файл с map между <nickname> -> <реален username> и nickname, ще ви улесни да изпращате съобщения на хората.

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

# Sending message?
#write $USER
#echo "hello"
```

### 12. Напишете shell script, който автоматично да попълва файла указател от предната задача по подадени аргументи: име на файла указател, пълно име на човека (това, което очакваме да е в /etc/passwd) и избран за него nickname.

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

USER="$(egrep -a "^([^:]*:){4}${NAME}(:|,)" ${PASSWD} | cut -d ':' -f 1)"

if [ -z "${USER}" ]; then
        echo "User with such name is not found"
        exit 3
fi

echo "${NICK} ${USER}" >> myAddressBook
exit 0
```

### 13. Напишете shell script, който да приема параметър име на директория, от която взимаме файлове, и опционално експлицитно име на директория, в която ще копираме файлове. Скриптът да копира файловете със съдържание, променено преди по-малко от 45 мин, от първата директория във втората директория. Ако втората директория не е подадена по име, нека да получи такова от днешната дата във формат, който ви е удобен. При желание новосъздадената директория да се архивира.

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

### 14. Да се напише shell скрипт, който получава при стартиране като параметър в командния ред идентификатор на потребител. Скриптът периодично (sleep(1)) да проверява дали потребителят е log-нат, и ако да - да прекратява изпълнението си, извеждайки на стандартния изход подходящо съобщение.

**NB! Можете да тествате по същият начин като в 05-b-4300.txt**

```bash

```

### 15. Да се напише shell скрипт, който валидира дали дадено цяло число попада в целочислен интервал. Скриптът приема 3 аргумента: числото, което трябва да се провери; лява граница на интервала; дясна граница на интервала. Скриптът да връща exit status:
* **3**, когато поне един от трите аргумента не е цяло число
* **2**, когато границите на интервала са обърнати
* **1**, когато числото не попада в интервала
* **0**, когато числото попада в интервала

```bash

```

### 16.
