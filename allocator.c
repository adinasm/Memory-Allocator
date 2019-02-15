// Copyright 2017 Smeu Adina

#include "./allocator.h"

// Initializeaza o arena ce are dimensiunea de N octeti
void initialize(unsigned char **arena, unsigned int N) {
    *arena = (unsigned char *) calloc (N, sizeof(unsigned char));
}

// Elibereaza memoria alocata la initializare
void finalize(unsigned char *arena) {
    free(arena);
}

// Afiseaza harta memoriei octet cu octet
void dump(unsigned char *arena, unsigned int N) {
    unsigned int i;

    for (i = 0; i < N; i++) {
        // Se afiseaza indicele curent in format hexazecimal, cu 8 cifre hexa
        // majuscule, apoi un tab
        if (!(i & 15)) {
            printf("%08X\t", i);
        }

        // Se afiseaza un octet in format hexazecimal, cu 2 cifre majuscule,
        // apoi un spatiu
        printf("%02X ", *(arena + i));

        // Se afiseaza un spatiu intre octetii 8 si 9 (daca nu s-a ajuns
        // la finalul arenei)
        if (!((i + 1) & 7) && (i + 1) & 15 && i + 1 < N) {
            printf(" ");
        }

        if (!((i + 1) & 15)) {
        printf("\n");
        }
    }

    if (i & 15) {
        printf("\n");
    }

    // Se afiseaza dimensiunea arenei, cu 8 cifre hexa majuscule
    printf("%08X\n", i);
}

// Aloca size octeti de memorie din arena
void alloc(unsigned char *arena, unsigned int N, unsigned int size) {
    int *prev_index = (int *)(arena);
    int *next_index = (int *)(arena + *prev_index);
    unsigned int free_space = 0;
    size += 3 * (sizeof(int));

    // Daca nu incape blocul in arena
    if (N - sizeof(int) < size) {
        printf("%d\n", 0);

        return;
    }

    // Daca arena este goala
    if (*prev_index == 0) {
        // Se modifica start index
        *prev_index = sizeof(int);

        // Modificari asupra zonei de date a blocului inserat
        *(prev_index + 1) = 0;
        *(prev_index + 2) = 0;
        *(prev_index + 3) = size;

        // Este afisat indicele inceputului zonei de date
        printf("%d\n", sizeof(int) * 4);

        return;
    }

    free_space = *prev_index - sizeof(int);

    // Se cauta loc liber intre 2 blocuri
    while (free_space < size && *next_index) {
        prev_index = next_index;
        next_index = (int *)(arena + *next_index);
        free_space = *prev_index - *(prev_index + 2) - *(next_index + 1);
    }

    // Se insereaza intre 2 blocuri
    if (free_space >= size) {
        int prev_size = sizeof(int);

        if (*(next_index + 1)) {
            prev_size = *(prev_index + 2);
        }

        // Modificari asupra zonei de date a blocului inserat, cat si
        // asupra zonelor vecinilor sai
        int *new_index = (int *)(arena + *(next_index + 1) + prev_size);
        *(new_index) = *prev_index;
        *(new_index + 1) = *(next_index + 1);
        *(new_index + 2) = size;
        *(next_index + 1) = *(next_index + 1) + prev_size;
        *prev_index = *(next_index + 1);

        // Este afisat indicele inceputului zonei de date
        printf("%d\n", *prev_index + sizeof(int) * 3);

        return;
    }

    // Se insereaza la finalul arenei
    if (!*next_index) {
        int next_size = *(next_index + 2) + *prev_index;
        free_space = N - next_size;

        if (free_space >= size) {
            // Modificari asupra zonei de date a blocului inserat, cat si
            // asupra zonelor vecinilor sai
            int *new_index = (int *)(arena + *next_index + next_size);
            *(new_index)= *next_index;
            *(new_index + 1) = *prev_index;
            *(new_index + 2) = size;
            *next_index = *prev_index + *(next_index + 2);

            // Este afisat indicele inceputului zonei de date
            printf("%d\n", *next_index + sizeof(int) * 3);

            return;
        }
    }

    // Blocul nu poate fi inserat
    printf("%d\n", 0);
}

// Elibereaza blocul de memorie al carei sectiuni de date incepe la pozitia
// index din arena
void free_block(unsigned char *arena, unsigned int index) {
    int *current_index = (int *)(arena + index - 3 * sizeof(int));
    int *prev_index = (int *)(arena + *(current_index + 1));
    int *next_index = (int *)(arena + *current_index);


    if (*current_index && *(current_index + 1)) {
        // Daca se afla intre 2 blocuri
        *prev_index = *current_index;
        *(next_index + 1) = *(current_index + 1);
    } else if (*current_index && !*(current_index + 1)) {
        // Daca este primul bloc din arena
        *prev_index = *current_index;
        *(next_index + 1) = 0;
    } else if (!*current_index && *(current_index + 1)) {
        // Daca este ultimul bloc din arena
        *prev_index = *current_index;
    } else {
        // Daca este unicul bloc din arena
        *prev_index = 0;
    }
}

// Seteaza la valoarea value size octeti consecutivi din arena, incepand cu
// pozitia index
void fill(unsigned char *arena, unsigned int index, unsigned int size,
unsigned int value) {
    unsigned int i;

    for (i = index; i < index + size; i++) {
        *(arena + i) = value;
    }
}

// Afiseaza numarul de regiuni libere si numarul de octeti liberi din arena
void show_free(unsigned char *arena, unsigned int N) {
    unsigned int nblocks = 0, nbytes = 0, free_space;

    int *prev_index = (int *)(arena);
    int *next_index = (int *)(arena + *prev_index);

    // Daca arena este goala
    if (!*prev_index) {
        printf("1 blocks (%d bytes) free\n", N - sizeof(int));
        return;
    }

    // Daca exista zona libera intre start index si primul bloc
    if (*prev_index != sizeof(int)) {
        nblocks++;
        nbytes += *prev_index - sizeof(int);
    }

    // Se cauta zonele libere dintre blocuri
    while (*next_index) {
        prev_index = next_index;
        next_index = (int *)(arena + *prev_index);

        free_space = *prev_index - *(next_index + 1) - *(prev_index + 2);

        if (free_space) {
            nblocks++;
            nbytes += free_space;
        }
    }

    free_space = N - *prev_index - *(next_index + 2);

    // Daca exista zona libera dupa ultimul bloc
    if (free_space) {
        nblocks++;
        nbytes += free_space;
    }

    printf("%d blocks (%d bytes) free\n", nblocks, nbytes);
}

// Afiseaza numarul de octeti din sectiunile de date
// Afiseaza eficienta utilizarii si fragmentarea (in procente)
void show_usage(unsigned char *arena, unsigned int N) {
    unsigned int occupied_blocks = 0, free_blocks = 0;
    unsigned int nbytes = 0, gestion_bytes = 0, free_space;
    float fragmentation, efficiency;

    int *prev_index = (int *)(arena);
    int *next_index = (int *)(arena + *prev_index);

    // Daca arena este goala
    if (!*prev_index) {
        printf("%d blocks (%d bytes) used\n", occupied_blocks, nbytes);
        printf("%d%c efficiency\n", 0, '%');
        printf("%d%c fragmentation\n", 0, '%');
        return;
    }

    // Daca exista zona libera intre start index si primul bloc
    if (*prev_index != sizeof(int)) {
        free_blocks++;
    }

    // Se parcurg toate blocurile
    while (*next_index) {
        prev_index = next_index;
        next_index = (int *)(arena + *prev_index);

        occupied_blocks++;
        nbytes += *(prev_index + 2);
        gestion_bytes += 3 * sizeof(int);

        free_space = *prev_index - *(next_index + 1) - *(prev_index + 2);

        if (free_space) {
            free_blocks++;
        }
    }

    // Pentru ultimul bloc
    occupied_blocks++;
    nbytes += *(next_index + 2);
    gestion_bytes += 3 * sizeof(int);

    free_space = N - *prev_index - *(next_index + 2);
    if (free_space) {
        free_blocks++;
    }

    // Se determina eficienta si fragmentarea
    nbytes -= gestion_bytes;
    efficiency = 100 * nbytes / (nbytes + gestion_bytes + sizeof(int));
    fragmentation = 100 * (free_blocks - 1) / occupied_blocks;

    printf("%d blocks (%d bytes) used\n", occupied_blocks, nbytes);
    printf("%.0f%c efficiency\n", efficiency, '%');
    printf("%.0f%c fragmentation\n", fragmentation, '%');
}

// Afiseaza zonele libere si alocate din arena
void show_allocations(unsigned char *arena, unsigned int N) {
    int *prev_index = (int *)(arena);
    int *next_index = (int *)(arena + *prev_index);
    unsigned int free_space;

    printf("OCCUPIED %d bytes\n", sizeof(int));

    // Daca arena este goala
    if (!*prev_index) {
        printf("FREE %d bytes\n", N - sizeof(int));
        return;
    }

    // Daca exista zona libera intre start index si primul bloc
    if (*prev_index - sizeof(int)) {
        printf("FREE %d bytes\n", *prev_index - sizeof(int));
    }

    // Se afiseaza blocurile si zonele libere dintre blocuri
    while (*next_index) {
        prev_index = next_index;
        next_index = (int *)(arena + *prev_index);

        free_space = *prev_index - *(next_index + 1) - *(prev_index + 2);

        printf("OCCUPIED %d bytes\n", *(prev_index + 2));

        if (free_space) {
            printf("FREE %d bytes\n", free_space);
        }
    }

    // Se afiseaza ultimul bloc
    printf("OCCUPIED %d bytes\n", *(next_index + 2));

    // Se afiseaza zona libera de la finalul arenei (daca exista)
    free_space = N - *prev_index - *(next_index + 2);
    if (free_space) {
        printf("FREE %d bytes\n", free_space);
    }
}
