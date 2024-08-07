CC=gcc
CFLAGS=-Wall -Werror
LDFLAGS=-L../can -lcan

sender: sender.c
	$(CC) $(CFLAGS) -o sender sender.c $(LDFLAGS)

clean:
	rm -f sender
