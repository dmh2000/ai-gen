CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -fPIC
AR = ar
ARFLAGS = rcs

TARGET = libcan.a
SRCS = can.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
