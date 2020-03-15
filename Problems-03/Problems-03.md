# 3. Обработка на текст

### 1. Сортирайте /etc/passwd лексикографски по поле UserID.
```bash
sort -t ':' -k 3 /etc/passwd
```

### 2. Сортирайте /etc/passwd числово по поле UserID. (Открийте разликите с лексикографската сортировка)
```bash
sort -t ':' -k 3 -n /etc/passwd
```

### 3. Изведете само 1-ва и 5-та колона на файла /etc/passwd спрямо разделител ":".
```bash
cut -d ':' -f 1,5  /etc/passwd
```

### 4. Изведете съдържанието на файла /etc/passwd от 2-ри до 6-ти символ.
```bash
head -c6 /etc/passwd | tail -c5
```

### 5. Намерете броя на символите в /etc/passwd. А колко реда има в /etc/passwd?
```bash
wc -m /etc/passwd
wc -l /etc/passwd
```

### 6. Извадете от файл /etc/passwd:
#### 6.1. първите 12 реда
```bash
head -n 12 /etc/passwd
```
#### 6.2. първите 26 символа
```bash
head -c 26 /etc/passwd
```
#### 6.3. всички редове, освен последните 4
```bash
head -n -4 /etc/passwd
```
#### 6.4. последните 17 реда
```bash
tail -n 17 /etc/passwd
```
#### 6.5. 13-я ред (или друг произволен, ако нямате достатъчно редове)
```bash
sed -n '13p' /etc/passwd
```
#### 6.6. последните 4 символа от 13-ти ред
```bash
sed -n '13p' /etc/passwd | tail -c4
```

### 7. Отпечатайте потребителските имена и техните home директории от /etc/passwd.
```bash
cut -d ':' -f1,6 /etc/passwd | tr ':' ' '
```

### 8. Отпечатайте втората колона на /etc/passwd, разделена спрямо символ '/'.
```bash
cut -d '/' -f2 /etc/passwd
```

### 9. Запаметете във файл в своята home директория резултатът от командата ls -l изпълнена за вашата home директорията. Сортирайте създадения файла по второ поле (numeric, alphabetically).
```bash
sort -k 2 -n output.txt -o output.txt
```

### 10. Отпечатайте 2 реда над вашия ред в /etc/passwd и 3 реда под него // може да стане и без пайпове
```bash
egrep $(whoami) -A 3 -B 2 /etc/passwd
```

### 11. Колко хора не се казват Ivan според /etc/passwd
```bash
egrep -v -c '\<Ivan\>' passwd.txt
```

### 12. Изведете имената на хората с второ име по-дълго от 7 (>7) символа според /etc/passwd
```bash
cut -d ':' -f 5 passwd.txt | cut -d ' ' -f 2 | tr -d ',' | egrep '.{7,}'
```

### 13. Изведете имената на хората с второ име по-късо от 8 (<=7) символа според /etc/passwd // !(>7) = ?
```bash
cut -d ':' -f 5 passwd.txt | cut -d ' ' -f 2 | tr -d ',' | egrep -v '.{7,}'
```

### 14. Изведете целите редове от /etc/passwd за хората от 03-a-5003
```bash
egrep '^[^:]*:[^:]*:[^:]*:[^:]*:[[:alnum:]]* +[[:alnum:]]{,7}[,:]+' passwd.txt
```

### 15. Намерете факултетния си номер във файлa /etc/passwd.
```bash
egrep -i 'kiril golov' passwd.txt | cut -d ':' -f 1 | tail -c +2
```

### 16. Запазете само потребителските имена от /etc/passwd във файл users във вашата home директория.

### 17. Колко коментара има във файла /etc/services ? Коментарите се маркират със символа #, след който всеки символ на реда се счита за коментар.
```bash
egrep -c '^[^#]*#+' /etc/services
```

### 18. Вижте man 5 services. Напишете команда, която ви дава името на протокол с порт естествено число N. Командата да не отпечатва нищо, ако търсения порт не съществува (например при порт 1337). Примерно, ако номера на порта N е 69, командата трябва да отпечати tftp.
```bash
egrep '^[[:alnum:]]*[[:space:]]+69/' /etc/services | cut -f 1 | sort | uniq
egrep '^[[:alnum:]]*[[:space:]]+27374/' /etc/services | cut -f 1 | sort | uniq
```

### 19. Колко файлове в /bin са shell script? (Колко файлове в дадена директория са ASCII text?)
```bash
file /bin/* | egrep -c 'shell script'
file /bin/* | egrep -c 'ASCII text'
```

### 20. Направете списък с директориите на вашата файлова система, до които нямате достъп. Понеже файловата система може да е много голяма, търсете до 3 нива на дълбочина. А до кои директории имате достъп? Колко на брой са директориите, до които нямате достъп?
```bash
find / -maxdepth 3 -type d 2>&1 | egrep 'Permission denied' | cut -d ':' -f 2 | cut -c5- | sed 's/.$//'
find / -maxdepth 3 -type d 2>&1 | egrep -v 'Permission denied'
find / -maxdepth 3 -type d 2>&1 | egrep -c 'Permission denied'
```

### 21. Създайте следната файлова йерархия.
```
/home/s...../dir1/file1
/home/s...../dir1/file2
/home/s...../dir1/file3
```

Посредством vi въведете следното съдържание:

**file1:**
```
1
2
3
```

**file2:**
```
s
a
d
f
```

**file3:**
```
3
2
1
45
42
14
1
52
```

### 22. Във file2 подменете всички малки букви с главни.
```bash
sed -i 's/[a-z]/\U&/g' file2
```

### 23. Във file3 изтрийте всички "1"-ци.
```bash
sed -i 's/1//g' file3
```

### 24. Изведете статистика за най-често срещаните символи в трите файла.
```bash
grep -o . file1 file2 file3  | cut -d ':' -f 2 | sort | uniq -c | sort -k 1 -n -r | head -n 1 | tr -s ' ' | cut -d ' ' -f 3
```

### 25. Направете нов файл с име по ваш избор, който е конкатенация от file{1,2,3}. Забележка: съществува решение с едно извикване на определена програма - опитайте да решите задачата чрез него.
```bash
cat file{1..3} > file
```

### 26. Прочетете текстов файл file1 и направете всички главни букви малки като запишете резултата във file2.
```bash
tr 'A-Z' 'a-z' < file1 > file2
```

### 27. Изтрийте всички срещания на буквата 'a' (lower case) в /etc/passwd и намерете броят на оставащите символи.
```bash
tr -d 'a' < passwd.txt | wc -m
```

### 28. Намерете броя на уникалните символи, използвани в имената на потребителите от /etc/passwd.
```bash
cut -d ':' -f 1 passwd.txt | egrep -o . | sort | uniq -c | egrep '^[[:space:]]*1 +' | tr -s ' ' | cut -d ' ' -f 3
```

### 29. Отпечатайте всички редове на файла /etc/passwd, които не съдържат символния низ 'ov'.
```bash
egrep -v -c 'ov' passwd.txt
```

### 30. Отпечатайте последната цифра на UID на всички редове между 28-ми и 46-ред в /etc/passwd.
```bash
head -n 46 passwd.txt | tail -n +28 | cut -d ':' -f 3 | rev | cut -c1
```

### 31. Отпечатайте правата (permissions) и имената на всички файлове, до които имате read достъп, намиращи се в директорията /tmp.
```bash
find /tmp -type f -readable -printf "%M %p\n" 2> /dev/null
```

### 32. Намерете имената на 10-те файла във вашата home директория, чието съдържание е редактирано най-скоро. На първо място трябва да бъде най-скоро редактираният файл. Намерете 10-те най-скоро достъпени файлове. (hint: Unix time)
```bash
find ~ -type f -printf "%T@ %p\n" 2> /dev/null | sort -n -r | head -n 10 | cut -d ' ' -f 2
find ~ -type f -printf "%A@ %p\n" 2> /dev/null | sort -n -r | head -n 10 | cut -d ' ' -f 2
```

### 33. Файловете, които съдържат C код, завършват на '.c'. Колко на брой са те във файловата система (или в избрана директория)? Колко реда C код има в тези файлове?
```bash
find / -type f -name '*.c' -printf "." 2> /dev/null | wc -m
find / -type f -name '*.c' 2> /dev/null -exec wc -l {} + | tail -n 1
```

### 34. Даден ви е ASCII текстов файл (например /etc/services). Отпечатайте хистограма на N-те (например 10) най-често срещани думи.
```bash
tr '[:space:]' '\n' < /etc/services | sed 's/[[:punct:]]/\n/g' | tr -s '\n' | sort | uniq -i -c | sort -n -r | head -n 10
```

### 35. Вземете факултетните номера на студентите от СИ и ги запишете във файл si.txt сортирани.
```bash
egrep -o '^s[0-9]+:' passwd.txt | egrep -o '[0-9]*' | sort -n > si.txt
```

### 36. За всеки логнат потребител изпишете "Hello, потребител", като ако това е вашият потребител, напишете "Hello, потребител - this is me!".
```bash
who | awk -v currUser=$(whoami) '{if($1 == currUser) {print "Hello,",$1,"- this is me!"} else {print "Hello,",$1}}'
```

### 37. Изпишете имената на студентите от /etc/passwd с главни букви.
```bash
egrep -a '^s[0-9]*:' passwd.txt | egrep -o '[[:alpha:]]+ [[:alpha:]]+' | sed 's/[[:lower:]]/\U&/g'
```

### 38. Shell Script-овете са файлове, които по конвенция имат разширение .sh. Всеки такъв файл започва с "#!<interpreter>" , където <interpreter> указва на операционната система какъв интерпретатор да пусне (пр: "#!/bin/bash", "#!/usr/bin/python3 -u"). Намерете всички .sh файлове и проверете кой е най-често използваният интерпретатор.
```bash
find / -type f -name '*.sh' 2> /dev/null -exec egrep -a '^#!' {} \; | sed 's/! \(.\)/!\1/g' | sed 's/ \(.\)*//g' | sort | uniq -c | sort -n -r | head -n 1
```

**Упътване: find намира всички файлове с разширение .sh във файловата система. Подава ги на egrep, който търси редове, започващи с "#!". След намиране на тези редове, с последващите команди sed премахваме евентуален интрервал между #! <interpreter>, както и всякакви флагове след самия интерпретатор. Накрая сортираме, с uniq -c взимаме броя на срещания на дадения интерпретатор, сортираме отново по брой, и извеждаме най-често използвания интерпретатор.**

### 39. Намерете 5-те най-големи групи подредени по броя на потребителите в тях.
```bash
```

### 40. Направете файл eternity. Намерете всички файлове, които са били модифицирани в последните 15мин (по възможност изключете .).  Запишете във eternity името на файла и часa на последната промяна.
```bash
find ~ -type f -mmin -15 2> /dev/null -fprintf eternity "%TX\t%p\n"
```

### 41. Копирайте файл /home/tony/population.csv във вашата home директория.

### 42. Използвайки файл population.csv, намерете колко е общото население на света през 2008 година. А през 2016?
```bash
cat population.csv | awk -M -F ',' '$3 ~ "2008" {sum += $4} END  {print sum}'
cat population.csv | awk -M -F ',' '$3 ~ "2016" {sum += $4} END  {print sum}'
```

### 43. Използвайки файл population.csv, намерете през коя година в България има най-много население.
```bash
egrep -a 'Bulgaria' population.csv | sort -t ',' -k 4 -n -r | head -n 1 | cut -d ',' -f 3
```

### 44. Използвайки файл population.csv, намерете коя държава има най-много население през 2016. А коя е с най-малко население? (Hint: Погледнете имената на държавите)
```bash
egrep -a ',2016,' population.csv | sort -t ',' -k 4 -n -r | head -n 1 | cut -d ',' -f 1
egrep -a ',2016,' population.csv | sort -t ',' -k 4 -n -r | tail -n 1 | cut -d ',' -f 1
```

### 45. Използвайки файл population.csv, намерете коя държава е на 42-ро място по население през 1969. Колко е населението й през тази година?
```bash
egrep -a ',1969,' population.csv | sort -t ',' -k 4 -n -r | head -n 42 | tail -n 1 | awk -F ',' '{print $1, $4}'
```

### 46. В home директорията си изпълнете командата
```bash
curl -o songs.tar.gz "http://fangorn.uni-sofia.bg/misc/songs.tar.gz"
```

### 47. Да се разархивира архивът songs.tar.gz в папка songs във вашата home директорията.
```bash
tar -xf songs.tar.gz -C songs
```

### 48. Да се изведат само имената на песните.
```bash
find songs -type f | egrep -o '\- .*\(' | egrep -o '[[:alpha:]]+.*[[:alpha:]]+'
```

### 49. Имената на песните да се направят с малки букви, да се заменят спейсовете с долни черти и да се сортират.
```bash
find songs -type f | egrep -o '\- .*\(' | egrep -o '[[:alpha:]]+.*[[:alpha:]]+' | sed 's/[[:upper:]]/\L&/g' | sed 's/ /_/g'
```

### 50. Да се изведат всички албуми, сортирани по година.
```bash
find songs -type f | egrep -o '\(.*\)' | tr -d '\(\)' | sort -t ',' -k 2 -n -r
```

### 51. Да се преброят/изведат само песните на Beatles и Pink.
```bash
find songs -type f | egrep -o '/.*\(' | egrep -o '[[:alpha:]]+.*[[:alpha:]]+' | egrep '^(Beatles|Pink) -'
```

### 52. Да се направят директории с имената на уникалните групи. За улеснение, имената от две думи да се напишат слято: Beatles, PinkFloyd, Madness
```bash
find songs -type f | egrep -o '/.* \-' | egrep -o '[[:alpha:]]+.*[[:alpha:]]+' | sort | uniq | tr -d ' ' | xargs -I{} mkdir songs/{}
```

### 53. Напишете серия от команди, които извеждат детайли за файловете и директориите в текущата директория, които имат същите права за достъп както най-големият файл в /etc директорията.
```bash
find . stat -perm $(find /etc -type f -printf "%s %m\n" 2> /dev/null | sort -n -r | head -n 1 | cut -d ' ' -f 2) -exec ls -l {} +
```

### 54. Дадени са ви 2 списъка с email адреси - първият има 12 валидни адреса, а вторията има само невалидни. Филтрирайте всички адреси, така че да останат само валидните. Колко кратък регулярен израз можете да направите за целта? Примери:

**Валидни email адреси (12 на брой):**
```
email@example.com
firstname.lastname@example.com
email@subdomain.example.com
email@123.123.123.123
1234567890@example.com
email@example-one.com
_______@example.com
email@example.name
email@example.museum
email@example.co.jp
firstname-lastname@example.com
unusually.long.long.name@example.com
```

**Невалидни email адреси:**
```
#@%^%#$@#$@#.com
@example.com
myemail
Joe Smith <email@example.com>
email.example.com
email@example@example.com
.email@example.com
email.@example.com
email..email@example.com
email@-example.com
email@example..com
Abc..123@example.com
(),:;<>[\]@example.com
just"not"right@example.com
this\ is"really"not\allowed@example.com
```

#### Решение:
```bash
egrep '^[[:alnum:]_]+([\.][[:alnum:]]+|[_\-][[:alnum:]]*)*@(([[:alnum:]]+[[:alnum:]\-]*[[:alnum:]]+\.)+[[:alnum:]]+|([0-9]+\.){3}[0-9]+)$' emails.txt
```

### 55. Запишете във файл next потребителското име на човека, който е след вас в изхода на who. Намерете в /etc/passwd допълнителната инфромация (име, специалност...) и също го запишете във файла next. Използвайки файла, изпратете му съобщение "I know who you are, информацията за студента"

**Hint: можете да използвате командата expr, за да смятате аритметични изрази. Например, ще получим 13, ако изпълним: expr 10 + 3**

**Бонус: "I know who you are, само името му"**

```bash
```