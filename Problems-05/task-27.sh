#!/bin/bash

SUM=0

while IFS= read -d ':' DIR; do
	COUNT=$(find "${DIR}" -type f -executable -printf '.' | wc -m)

	SUM=$((SUM+COUNT))
done < <(echo "${PATH}:")

echo ${SUM}

exit 0
