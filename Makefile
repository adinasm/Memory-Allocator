# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Here is the name for executable
EXE = allocator

build: allocator.c main.c
	$(CC) $(CFLAGS) $^ -o $(EXE)

.PHONY: clean
clean:
	rm -f *.o $(EXE) 
