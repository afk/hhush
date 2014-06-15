#!/bin/bash

begin=$(LC_ALL=C date +"%a %b %d %T %Y")

./scripts/runTest.sh tPipe-grep-date

exitcode=$?

max=$(echo $(date +'%s')-$(date -d "$begin" +'%s')+1 | bc)

for (( i=1; i<=$max; i++ ))
do
	if [ $(grep -ch "$begin" output/ausgabe_tPipe-grep-date) == 1 ]; then
		break
	else
		if [ "$i" == "$max" ]; then
			break
		fi
		begin=$(LC_ALL=C date -d "$begin +1 second" +"%a %b %d %T %Y")
	fi
done

cp results/tPipe-grep-dateSource results/tPipe-grep-date
sed -i s@"\$DATE"@"$begin"@g results/tPipe-grep-date

exit $exitcode
