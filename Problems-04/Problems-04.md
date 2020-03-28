# 4. Процеси

### 1. Намерете командите на 10-те най-стари процеси в системата.
```bash
ps -eo etimes,cmd | tail -n +2 | sort -n -r | head | awk '{print $2}'
ps -eo cmd --sort=start_time | head -n 11
```

### 2. Намерете PID и командата на процеса, който заема най-много виртуална памет в системата.
```bash
ps -eo vsz,pid,cmd | tail -n +2 | sort -n -r | head -n 1 | awk '{print $2, $3}'
ps -eo pid,cmd --sort=vsz | head -n 2
```

### 3. Изведете командата на най-стария процес
```bash
ps -eo etimes,cmd | tail -n +2 | sort -n -r | head -n 1 | awk '{print $2}'
ps -eo cmd --sort=start_time | head -n 2
```

### 4. Намерете колко физическа памет заемат всички процеси на потребителската група students.
```bash
ps -o rsz,cmd --group "students"
```

### 5. Изведете имената на потребителите, които имат поне 2 процеса, чиято команда е vim
```bash
ps -o user -C "vim" | tail -n +2 | sort | uniq -c | awk '$1 > 1 {print $2}'
```

### 6. Изведете имената на потребителите, които не са логнати в момента, но имат живи процеси
```bash
ps -eo user | sort | uniq | fgrep -v "$(who | awk '{print $1}' | tr '\n' '\|' | sed 's/.$//')"
```

### 7. Намерете колко физическа памет заемат осреднено всички процеси на потребителската група students. Внимавайте, когато групата няма нито един процес.
```bash
ps -o rsz --group "students" 2> /dev/null | awk '{sum += $1} END {if (NR > 0) { print sum/NR }}'
```

### 8. Намерете всички PID и техните команди (без аргументите), които нямат tty, което ги управлява. Изведете списък само с командите без повторения.
```bash
ps -eo tty,pid,cmd | grep -v "^?" | awk '{print $2, $3}'
```

### 9. Да се отпечатат PID на всички процеси, които имат повече деца от родителския си процес.
```bash

```
