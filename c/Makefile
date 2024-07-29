CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -L../can
LIBS = -lcan

TARGET = sender
SRCS = sender.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
