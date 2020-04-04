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
```bash

```

### 9. Да се напише shell скрипт, който чете от стандартния вход име на файл и символен низ, проверява дали низа се съдържа във файла и извежда на стандартния изход кода на завършване на командата с която сте проверили наличието на низа.
**NB! Символният низ може да съдържа интервал (' ') в себе си.**
```bash

```
