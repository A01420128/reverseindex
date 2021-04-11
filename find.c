#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct pregistro {
  char word[50];
  int ptr;
} pregistro;

typedef struct sregistro {
  int line;
  int next;
} sregistro;

int binary(char * word, long in, long fin, FILE *fptr) {
    // caso b si no lo encuentras
    printf("%ld, %ld\n", in, fin);
    if (in > fin) {
        return -1;
    }
    long mitad = (fin + in) / 2;
    printf("long: %ld\n", mitad);

    pregistro p;
    fseek(fptr, sizeof(pregistro) * mitad, SEEK_SET);
    fread(&p, sizeof(pregistro), 1, fptr);
    int cmp = strcmp(word, p.word);
    printf("%s vs %s\n", word, p.word);
    if (cmp == 0) {
        return p.ptr;
    }

    if (cmp > 0) {
        return binary(word, mitad+1, fin, fptr);
    } else {
        return binary(word, in, mitad-1, fptr);
    }
}

int main() {
    char req[50];
    printf("Palabra? \n");
    scanf("%s", req);
    printf("Buscando %s...\n", req);

    // buscar en pregistro
    FILE *fptr;
    char file[] = "pregistro";
    fptr = fopen(file, "rb");
    if (fptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // Ver tamano de archivo
    fseek(fptr, 0, SEEK_END);
    long pregsize = ftell(fptr);
    long spreg = pregsize / sizeof(pregistro);
    printf("Numero pregistros: %ld\n", spreg);
    // Binary search
    int ptr = binary(req, 0, spreg, fptr);
    printf("Ptr: %d\n", ptr);


    fclose(fptr);
    return 0;
}
