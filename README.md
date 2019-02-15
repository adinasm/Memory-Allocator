# Memory Allocator


### initialize
- Initializeaza o arena ce are dimensiunea de N octeti, aceasta fiind prima
comanda apelata.

### finalize
- Elibereaza memoria alocata la initializare, aceasta fiind ultima comanda
apelata.

### dump
- Afiseaza harta memoriei octet cu octet, in format hexazecimal.
- Se afiseaza cate 16 octeti pe fiecare linie, iar la final se afiseaza
si dimensiunea arenei.


### alloc
- Aloca SIZE octeti de memorie din arena, alegandu-se prima zona libera valida,
intr-o cautare de la stanga la dreapta.
- Afiseaza indexul sectiunii de date a blocului alocat in arena.
- Afiseaza 0 daca nu a fost gasita nicio zona libera suficient de mare in arena. 


### free_block
- Elibereaza blocul de memorie al carei sectiuni de date incepe la pozitia
INDEX din arena.


### fill
- Seteaza la valoarea VALUE SIZE octeti consecutivi din arena, incepand cu
pozitia INDEX.


### show_free
- Afiseaza numarul de regiuni libere si numarul de octeti liberi din arena sub
forma:
		<nblocks> blocks (<nbytes> bytes) free


### show_usage
- Afiseaza numarul de octeti din sectiunile de date.
- Afiseaza eficienta utilizarii si fragmentarea (in procente).


### show_allocations
- Afiseaza pe cate o linie, zonele libere si alocate din arena.