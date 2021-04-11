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
    if (in > fin) {
        return -1;
    }
    long mitad = (fin + in) / 2;

    pregistro p;
    fseek(fptr, sizeof(pregistro) * mitad, SEEK_SET);
    fread(&p, sizeof(pregistro), 1, fptr);
    int cmp = strcmp(word, p.word);
    if (cmp == 0) {
        printf("Encontro %s en:\n", word);
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
    FILE *fptr = fopen("pregistro", "rb");
    if (fptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // Ver tamano de archivo
    fseek(fptr, 0, SEEK_END);
    long pregsize = ftell(fptr);
    long spreg = pregsize / sizeof(pregistro);
    // Binary search
    int ptr = binary(req, 0, spreg, fptr);
    if (ptr < 0) {
        printf("Palabra no encontrada.\n");
        return 0;
    }

    // Search lines in sregistro
    FILE *sfptr = fopen("sregistro", "rb");
    if (sfptr == NULL) {
        fprintf(stderr, "ERROR");
    }
    // leemos record hasta encrontrar -1 
    sregistro s;
    fseek(sfptr, sizeof(sregistro) * ptr, SEEK_SET);
    fread(&s, sizeof(sregistro), 1, sfptr);
    printf("%d\t", s.line);
    while (s.next != -1) {
        ptr = s.next;
        fseek(sfptr, sizeof(sregistro) * ptr, SEEK_SET);
        fread(&s, sizeof(sregistro), 1, sfptr);
        printf("%d\t", s.line);
    }
    putchar('\n');

    fclose(sfptr);
    fclose(fptr);
    return 0;
}
