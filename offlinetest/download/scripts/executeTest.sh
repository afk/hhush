#!/bin/bash

echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo ""
echo "Running test $1 ..."
echo ""

if test -e results/$1.sh; then

	./results/$1.sh

else

	./scripts/runTest.sh $1

fi

valgrindCode=$?
resultCode=0

echo -e "  2. Test results:\n"

if [ "$2" == "1" ]; then

	echo "Left side: your result | Right side: expected result ( | marks differing lines)"
	echo ""

	diff -y -W 200 output/ausgabe_$1 results/$1

else

	if [[ $1 == tLS* ]]
	then
		if diff -b -B -i -q -w <(sort output/ausgabe_$1) <(sort results/$1) > /dev/null; then
			echo -e "    Your result matches the target.\n"
		else
			echo -e "    Your result differs from the target.\n    Use \"./scripts/executeTest.sh $1 1\" to check your result.\n"
			resultCode=1
		fi
	else
		if diff -b -B -i -q output/ausgabe_$1 results/$1 > /dev/null; then
			echo -e "    Your result matches the target.\n"
		else
			echo -e "    Your result differs from the target.\n    Use \"./scripts/executeTest.sh $1 1\" to check your result.\n"
			resultCode=1
		fi
	fi
fi

if [[ $valgrindCode == 1 || $resultCode == 1 ]]; then
	echo -e "Test failed!\n"
	exit 1
else
	echo -e "Test succesful!\n"
	exit 0
fi
