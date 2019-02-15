// Copyright 2017 Smeu Adina

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cmd_read(char cmd[100], char *arg_cmd);
void initialize(unsigned char **arena, unsigned int N);
void finalize(unsigned char *arena);
void dump(unsigned char *arena, unsigned int N);
void alloc(unsigned char *arena, unsigned int N, unsigned int size);
void free_block(unsigned char *arena, unsigned int index);
void fill(unsigned char *arena, unsigned int index, unsigned int size,
unsigned int value);
void show_free(unsigned char *arena, unsigned int N);
void show_usage(unsigned char *arena, unsigned int N);
void show_allocations(unsigned char *arena, unsigned int N);

#endif  // ALLOCATOR_H
