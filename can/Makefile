CC=gcc
CFLAGS=-Wall -Werror
AR=ar
ARFLAGS=rcs

all: libcan.a

libcan.a: can.o
	$(AR) $(ARFLAGS) $@ $^

can.o: can.c can.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o libcan.a
