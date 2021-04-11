#include <stdio.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 81

typedef struct pregistro {
  char word[50];
  int ptr;
} pregistro;

typedef struct pentry {
    struct pregistro *p;
    struct pentry *sig;
} pentry;

typedef struct sregistro {
  int line;
  int next;
} sregistro;

void insertar(pentry *pe,pentry** raiz ) {
  // la lista esta vacía?
  if(*raiz == NULL) {
    *raiz = pe;
    return; 
  }
  // *raiz != NULL la lista no está vacía
  // va al comienzo de la lista?
  if(strcmp(pe->p->word, (*raiz)->p->word) < 0) {
    pe->sig = *raiz;
    *raiz = pe;
    return;
  }
  // no es el pregistroo de la lista
  pentry *pe1 = *raiz;
  pentry *pe2 = pe1->sig;
  while(pe2 != NULL && strcmp(pe->p->word, pe2->p->word) > 0) {
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

int listfind(char word[50], pentry** root) {
    pentry *this = *root;
    while (this != NULL) {
        if (strcmp(word, this->p->word) == 0) {
            return this->p->ptr;
        }
        this = this->sig;
    }
    return -1;
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

    FILE *pfptr = fopen("pregistro", "wb+");
    if (pfptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    FILE *sfptr = fopen("sregistro", "wb+");
    if (pfptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // lista de pregistro
    pentry * raiz = NULL;
    int ultimo_sreg = -1;

    // Leer linea
    char line[MAX_LINE];
    int nline = 0;
    while (cfgets(line, MAX_LINE, fptr) != NULL) {
        nline++;
        printf("%s\n", line);
        // separar palabra
        char delim[] = " '\".-_`?(),!:;[]";
        char *tofree, *word, *string;
        string = line;
        while ((word = strsep(&string, delim)) != NULL) {
            if (word[0] != '\0' && word[0] != '\n') {
                // convertir a lower case
                strlower(word, MAX_LINE);
                // buscar palabara en pregistro
                int sptr;
                if ((sptr = listfind(word, &raiz)) >= 0) {
                    // ya esta word en pregistro
                    // leemos record hasta encrontrar -1 
                    sregistro s;
                    fseek(sfptr, sizeof(sregistro) * sptr, SEEK_SET);
                    fread(&s, sizeof(sregistro), 1, sfptr);
                    while (s.next != -1) {
                        sptr = s.next;
                        fseek(sfptr, sizeof(sregistro) * sptr, SEEK_SET);
                        fread(&s, sizeof(sregistro), 1, sfptr);
                    }
                    // Tenemos sregistro de word con -1
                    sregistro nuevos;
                    nuevos.line = nline;
                    nuevos.next = -1;
                    // Actualizar registro que tenia -1;
                    s.next = ultimo_sreg + 1;
                    fwrite(&s, sizeof(sregistro), 1, sfptr);
                    // Insertar nuevo registro en sregistro
                    fseek(sfptr, 0, SEEK_END);
                    ultimo_sreg++;
                    fwrite(&nuevos, sizeof(sregistro), 1, sfptr);
                } else {
                    // no esta en plista

                    // insertar nuevo sregistro al final
                    sregistro nsreg;
                    nsreg.line = nline;
                    nsreg.next = -1;
                    fseek(sfptr, 0, SEEK_END);
                    fwrite(&nsreg, sizeof(sregistro), 1, sfptr);

                    // Crear pregistro
                    pregistro *npreg; // TODO: Malloc?
                    npreg = (pregistro *) malloc(sizeof(pregistro));
                    strcpy(npreg->word, word);
                    npreg->ptr = ultimo_sreg + 1;
                    ultimo_sreg++;

                    // Creamos nuevo elemnto de plista
                    pentry *nentry;
                    nentry = (pentry *) malloc(sizeof(pentry));
                    nentry->p = npreg;
                    nentry->sig = NULL;
                    insertar(nentry, &raiz);
                }
            }
        }
    }
    fclose(fptr); // dejamos de leer alice
    fclose(sfptr); // dejamos de escribir leer sregistro

    // escribir pregistrolista ordenado en pfptr
    if (raiz == NULL)
        return EXIT_FAILURE;

    pentry * curr = raiz;
    int pi = 0;
    while (curr != NULL) {
        // fwrite en pft
        fwrite(&curr->p, sizeof(pregistro), 1, pfptr);
        printf("%s\n", curr->p->word);
        curr = curr->sig;
        pi++;
    }
    fclose(pfptr);
    return EXIT_SUCCESS;


    /*
    for (int i = 0; i < sizepregistro + 1; i++) {
        pregistro p;
        char w[50] = "entry";
        strcpy(p.word, w);
        p.ptr = i;
        fwrite(&p, sizeof(pregistro), 1, pfptr);
    }
    */

    // Binary search
    // leer el tamano del archivo
    // 0 tamano
    /*
    for (int i = 0; i < sizepregistro + 1; i++) {
        fseek(pfptr, sizeof(pregistro) * i, SEEK_SET);
        pregistro p2;
        fread(&p2, sizeof(pregistro), 1, pfptr);
        printf("Entry: %s, ptr: %d\n", p2.word, p2.ptr);
    }
    */
}
