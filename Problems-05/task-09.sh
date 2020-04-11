#!/bin/bash

read -p "Enter full path to file: " FILENAME
read -p "Enter phrase to search for: " NEEDLE

grep -qsF "${NEEDLE}" "${FILENAME}"
echo $?
