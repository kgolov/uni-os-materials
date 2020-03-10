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
kiril@kiril-laptop:~/Desktop/OS/test$ find . -type f -size 0 -exec rm -f {} +
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
