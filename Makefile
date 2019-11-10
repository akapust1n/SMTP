CC=gcc
CFLAGS=-Wall  -std=c99
LFLAGS = -lpthread -lrt
SOURCESSERVER := $(shell find server -name '*.c')
INCCOMON=-Icommon
SOURCESCOMMON:= $(shell find common -name '*.c')

all: serverO

serverO: $(SOURCESSERVER)
	$(CC) $(CFLAGS) $(SOURCESSERVER) $(SOURCESCOMMON) $(INCCOMON) $(LFLAGS) -o serverO

clean:
	rm -rf *.o
