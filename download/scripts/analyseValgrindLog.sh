#!/bin/bash

errors=0
defLost=0
indirLost=0
posLost=0
reachable=0

while read line
do

	if [ "$(echo $line | awk '{print match($line, ".*ERROR SUMMARY.*")}')" == 1 ]; then
		errors=$(($errors+$(echo $line | awk '{print $4}')))
	elif  [ "$(echo $line | awk '{print match($line, ".*definitely lost\\:.*")}')" == 1 ]; then
		defLost=$(echo $line | awk '{print $4}')
	elif  [ "$(echo $line | awk '{print match($line, ".*indirectly lost\\:.*")}')" == 1 ]; then
		indirLost=$(echo $line | awk '{print $4}')
	elif  [ "$(echo $line | awk '{print match($line, ".*possibly lost\\:.*")}')" == 1 ]; then
		posLost=$(echo $line | awk '{print $4}')
	elif  [ "$(echo $line | awk '{print match($line, ".*still reachable\\:.*")}')" == 1 ]; then
		reachable=$(echo $line | awk '{print $4}')
	fi

done < output/valgrind_$1.log

echo -e "             Errors:    $errors
    definitely lost:    $defLost Bytes
    indirectly lost:    $indirLost Bytes
      possibly lost:    $posLost Bytes
    still reachable:    $reachable Bytes\n
    Further Informations: Please check file output/valgrind_$1.log for more information!\n"
