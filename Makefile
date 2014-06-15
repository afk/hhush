CC=gcc
CFLAGS=-Wall -Wextra -std=c99

all:
	$(CC) $(CFLAGS) hhush.c -o hhush

tests:
	./offlinetest.sh

clean:
	rm -rf hhush .hhush.hist* data output results scripts tests