#include <stdio.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 81

typedef struct primer {
  char word[50];
  int ptr;
} primer;

typedef struct pentry {
    char name[50];
    primer p;
    struct pentry *sig;
} pentry;

typedef struct segundo {
  int line;
  int next;
} segundo;

void insertar(pentry *pe,pentry** raiz ) {
  // la lista esta vacía?
  if(*raiz == NULL) {
    *raiz = pe;
    return; 
  }
  // *raiz != NULL la lista no está vacía
  // va al comienzo de la lista?
  if(strcmp(pe->nombre, (*raiz)->nombre) < 0) {
    pe->sig = *raiz;
    *raiz = pe;
    return;
  }
  // no es el primero de la lista
  pentry *pe1 = *raiz;
  pentry *pe2 = pe1->sig;
  while(pe2 != NULL && strcmp(pe->nombre, pe2->nombre) > 0) {
    pe1 = pe2;
    pe2 = pe2->sig;
  }
  if(pe2 != NULL) {
    pe->sig = pe2;
    pe1->sig = pe;
    return;
  }
  // va al final
  pe1->sig = pe;

}


char * cfgets(char * line, int s, FILE * ptr) {
    char* res = fgets(line, s, ptr);
    if (res != NULL)
        line[strlen(line) - 1] = '\0';
    return res;
}

void strlower(char * str, int size) {
    int i = 0;
    while (str[i] != '\0' && i < size) {
        str[i] = tolower(str[i]);
        i++;
    }
}

int main() {
    // Leer alice
    FILE *fptr;
    char file[] = "alice.txt";
    fptr = fopen(file, "r");
    if (fptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    FILE *pfptr = fopen("primer", "r+");
    if (pfptr == NULL) {
        fprintf(stderr, "ERROR");
    }
    // arr arrprimer
    int sizeprimer = 20;
    primer * arrprimer = malloc(sizeof(primer) * 20);
    //  linked list sortedprimer
    // name: ""
    // primer: name
    //          location
    // next:

    // Leer linea
    char line[MAX_LINE];
    while (cfgets(line, MAX_LINE, fptr) != NULL) {
        printf("%s\n", line);
        // separar palabra
        char delim[] = " '\".-`?(),!:;";
        char *tofree, *word, *string;
        string = line;
        while ((word = strsep(&string, delim)) != NULL) {
            if (word[0] != '\0' && word[0] != '\n') {
                // convertir a lower case
                strlower(word, MAX_LINE);
                printf("%s\n", word);
                // buscar palabara en primer
                primer p;
                strcpy(p.word, word);
                p.ptr = 0;
                int i = sizeprimer;
                while (i > 0; // ) {
                // arr.word > word
                    i--;
                }
            }


                // si lo econtramos nos vamos al segundo
                // si no lo tenemos que escribir
                // lo insertionsort al meterlo en arrprimer
            }
        }
    }
    fclose(fptr);
    // escribir arrprimer ordenado en pfptr
    for (int i = 0; i < sizeprimer + 1; i++) {
        primer p;
        char w[50] = "entry";
        strcpy(p.word, w);
        p.ptr = i;
        fwrite(&p, sizeof(primer), 1, pfptr);
    }

    // Binary search
    // leer el tamano del archivo
    // 0 tamano
    /*
    for (int i = 0; i < sizeprimer + 1; i++) {
        fseek(pfptr, sizeof(primer) * i, SEEK_SET);
        primer p2;
        fread(&p2, sizeof(primer), 1, pfptr);
        printf("Entry: %s, ptr: %d\n", p2.word, p2.ptr);
    }
    */

    fclose(pfptr);
    return EXIT_SUCCESS;
}
