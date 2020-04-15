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
