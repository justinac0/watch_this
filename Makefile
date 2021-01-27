CC=clang
CFLAGS=-Wall -Wextra -g -std=c99

all: main.c
	$(CC) $(CFLAGS) main.c -o watch_this_example

run:
	./watch_this_example
