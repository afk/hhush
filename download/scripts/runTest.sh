exitcode=0

while read line
do

	a="$(valgrind --error-exitcode=127 --leak-check=full --log-file=output/valgrind_$1.log --show-reachable=yes "$line")"

done < tests/$1

result=$?

echo -e "  1. Valgrind result:\n"

if [ "$result" == "127" ];
then

	./scripts/analyseValgrindLog.sh $1
	exitcode=1

else

	echo -e "    No memory leak occured.\n"

fi

while read line
do

	if [ "$line" != "./hhush" ]; then

		if [ "$line" != "exit" ]; then

			a="${a/\$ /\$###$line\n}"

		else

			a="${a/\$ /\$###$line}"

		fi

	fi

done < tests/$1

echo -e "${a//###/ }" >output/ausgabe_$1

exit $exitcode
