CC=gcc
CFLAGS=-Wall  -std=c99
SOURCESSERVER := $(shell find server -name '*.c')

all: serverO

serverO: $(SOURCESSERVER)
	$(CC) $(CFLAGS) $(SOURCESSERVER) -o serverO

clean:
	rm -rf *.o
