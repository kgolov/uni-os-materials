# Примерни задачи от контролно 1

## Условията на задачите се намират [тук](Exam-Problems.pdf)

### Зад. 1
```bash
egrep -c '^[^a-w]*[02468]+[^a-w]*$' philip-j-fry.txt
```

### Зад. 2

### Зад. 3

### Зад. 4

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
