CFLAGS = -std=c11 -Wall -Werror -Wextra -Wno-unused-parameter -I./include -I../common/include/ 

CC = gcc

LDFLAGS = -lpcre2-8 -lopts

LD = $(CC)

sources = $(wildcard src/*.c)
objs = $(patsubst src/%.c, ./obj/%.o, $(sources))

client: mkdirs compile_client

compile_client:  $(objs)
	$(LD) -o smtp-client $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

mkdirs:
	mkdir -p ./obj

clean:
	rm -f smtp-client
	rm -rf ./obj
