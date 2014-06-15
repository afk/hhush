#!/bin/bash

export PATH=/usr/local/bin:$PATH

clear;

#set -e

echo "################################################################"
echo ""
echo "Check Prerequisites"
echo ""
echo "################################################################"
echo ""
echo "Check for GCC"
if ! type gcc >/dev/null ; then
	echo 'ERROR: gcc not found!'
	exit 1
fi
echo "  Done"
echo "Check for Valgrind"
if ! type valgrind >/dev/null ; then
	echo 'ERROR: valgrind not found!'
	exit 1
fi
echo "  Done"
echo "Check if source code exists (hhush.c)"
if ! test -f hhush.c ; then
	echo 'ERROR: Could not find source code (hhush.c)'
	exit 1
fi
echo "  Done"
echo ""
echo "################################################################"
echo ""
echo "Compiling"
echo ""
echo "################################################################"

echo ""
echo "Errors:"
if gcc -w -std=c99 hhush.c -o hhush; then
echo ""
echo "Successful."
echo ""

else

echo ""
echo "ERROR: Compiling failed"
echo ""
echo "################################################################"
exit 1;

fi

echo "################################################################"
echo ""
echo "Downloading/Creating Test Cases"
echo ""
echo "################################################################"
echo ""

if ! test -e tests || ! test -e results || ! test -e data || ! test -e scripts; then

	echo 'Downloading test cases ...'
		cp -r download/* .
		cp download/.hhush.hist.original .
	echo '  Done'

	home=$(pwd)

	echo 'Altering test cases ...'
	for file in $(ls results/); do
		sed -i s@"\$HOME"@"$home"@g results/$file
	done

	for file in $(ls tests/); do
		sed -i s@"\$HOME"@"$home"@g tests/$file
	done
	echo '  Done'

else

	echo "Not necessary."

fi

echo ""
echo "################################################################"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "################################################################"
echo ""
echo "Running tests..."
echo ""
echo "################################################################"
echo ""
echo "Check used libraries"
./scripts/cFileCheck.sh
echo ""

if ! test -e output; then
	mkdir output
fi



cat .hhush.hist.original > .hhush.histfile

./scripts/executeTest.sh tPipe-grep-history

failedCounter=$?
testCounter=1

for t in $(ls tests/); do

	if [[ "$t" != 'tPipe-grep-history' ]]
	then
		./scripts/executeTest.sh $t
		failedCounter=$(($failedCounter+$?))
		testCounter=$(($testCounter+1))
	fi
done

echo -e "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"

echo -e "Conclusion: $failedCounter out of $testCounter tests failed.\n"

echo -e "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"

exit 1
