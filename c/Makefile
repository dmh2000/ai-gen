CC=gcc
CFLAGS=-Wall -Werror -I../can -std=c11 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-L../can -lcan

sender: sender.c
	$(CC) $(CFLAGS) -o sender sender.c $(LDFLAGS)

clean:
	rm -f sender
