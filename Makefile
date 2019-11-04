CC=gcc
CFLAGS=-Wall  -std=c99
LFLAGS = -lpthread
SOURCESSERVER := $(shell find server -name '*.c')

all: serverO

serverO: $(SOURCESSERVER)
	$(CC) $(CFLAGS) $(SOURCESSERVER) $(LFLAGS) -o serverO

clean:
	rm -rf *.o
