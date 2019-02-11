EXEC=shell.exe
SOURCES=shell.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -std=gnu99 -g
LFLAGS=-lm
 
.PHONY: default clean
 
default: $(EXEC)
 
shell.o: shell.c shell.h
main.o: main.c shell.c shell.h
 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~