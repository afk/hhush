#!/bin/bash



if ! $(diff -q <(echo "$(ls | grep *.c)") <(echo "hhush.c") > /dev/null);
then
    echo "  Folder should contain only one source file which is named hhush.c"

else
    badIncludes=$(grep -vxFf results/goodinc.c <(echo "$(grep '[[:space:]]*#include' hhush.c | sed 's/[[:space:]]*//g'| sed 's/\.h\>[[:print:]]*/\.h\>/g' )") )


    if diff -b -B -i -q -w <(echo "$badIncludes") <(echo "") > /dev/null;
    then
	    echo "  Header check successful"
    else
        echo -e "  The following headers are not allowed:\n\n$badIncludes\n"
        echo "  Only use the following headers:"
        echo "    ANSI-C:
    assert.h    ctype.h     errno.h     float.h    
    limits.h    locale.h    math.h      setjmp.h
    signal.h    stdarg.h    stddef.h    stdio.h
    stdlib.h    string.h    time.h
    
    UNIX:
    dirent.h    unistd.h    sys/types.h"

    fi
fi


