CC=gcc
CFLAGS=-std=c99

all:
	$(CC) $(CFLAGS) hhush.c -o hhush

debug:
	$(CC) $(CFLAGS) -g -Wall -Wextra hhush.c -o hhush

debugmac:
	$(CC) $(CFLAGS) -Wall -Wextra hhush.c -o hhush

clean:
	rm hhush