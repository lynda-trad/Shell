EXEC=shell
SOURCES=shell.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -Werror -std=gnu99 -g
LFLAGS=-lm
 
.PHONY: default clean
 
default: $(EXEC)
 
 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)
 
 
shell.exe :
	$(CC) $(CFLAGS) shell.c -o shell.exe
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~