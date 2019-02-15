// Copyright 2017 Smeu Adina

#include "./allocator.h"

int main() {
    char cmd[100];
    char *arg_cmd = NULL;

    cmd_read(cmd, arg_cmd);

    return 0;
}

void cmd_read(char cmd[100], char *arg_cmd) {
    unsigned int N, size, index, value;
    char *info;
    const char *delimiters = " \n";
    unsigned char *arena = NULL;

    while (fgets(cmd, 100, stdin) != NULL) {
        arg_cmd = strtok(cmd, delimiters);

        if (!strcmp(arg_cmd, "INITIALIZE")) {
            arg_cmd = strtok(NULL, delimiters);
            N = atoi(arg_cmd);

            initialize(&arena, N);
        } else if (!strcmp(arg_cmd, "FINALIZE")) {
            finalize(arena);
            return;
        } else if (!strcmp(arg_cmd, "DUMP")) {
            dump(arena, N);
        } else if (!strcmp(arg_cmd, "ALLOC")) {
            arg_cmd = strtok(NULL, delimiters);
            size = atoi(arg_cmd);

            alloc(arena, N, size);
        } else if (!strcmp(arg_cmd, "FREE")) {
            arg_cmd = strtok(NULL, delimiters);
            index = atoi(arg_cmd);

            free_block(arena, index);
        } else if (!strcmp(arg_cmd, "FILL")) {
            arg_cmd = strtok(NULL, delimiters);
            index = atoi(arg_cmd);

            arg_cmd = strtok(NULL, delimiters);
            size = atoi(arg_cmd);

            arg_cmd = strtok(NULL, delimiters);
            value = atoi(arg_cmd);

            fill(arena, index, size, value);
        } else if (!strcmp(arg_cmd, "SHOW")) {
            info = strtok(NULL, delimiters);

            if (!strcmp(info, "FREE")) {
                show_free(arena, N);
            } else if (!strcmp(info, "USAGE")) {
                show_usage(arena, N);
            } else if (!strcmp(info, "ALLOCATIONS")) {
                show_allocations(arena, N);
            }
        }
    }
}
