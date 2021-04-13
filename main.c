/*
PARTE 2
Realizarán un programa en lenguaje C que implemente un archivo invertido. 
Su implementación constará en realidad de dos archivos binarios de acceso 
aleatorio y actualizables

Usamos una lista ligada para guardar las palabras del primer archivo y hacer
el insertion sort. Al final imprimimos la lista en el primer archivo

Nombre primer archivo: pregistro
Nombre segundo archivo:spregistro

Por:
Jose Javier Tlacuilo Fuentes A01420128
Andrea Alessandra Villarruel Sernas A01656255
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 501 // TODO: Max line for a file.

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

// Convertir un string a lowercase.
void strlower(char * str, int size) {
    int i = 0;
    while (str[i] != '\0' && i < size) {
        str[i] = tolower(str[i]);
        i++;
    }
}

int main() {
    // Preguntar archivo.
    char filename[51];
    printf("Cual es el nombre del archivo?\n");
    scanf("%s", filename);

    // Leer archivo.
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // Abrir o crear segundo archivo con registros por linea.
    FILE *sfptr = fopen("sregistro", "wb+");
    if (sfptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // lista de pregistros, registros para el primer archivo.
    pentry * raiz = NULL;
    int ultimo_sreg = -1;

    // Leer linea
    char line[MAX_LINE];
    int nline = 0;
    while (cfgets(line, MAX_LINE, fptr) != NULL) { // Para cada linea
        nline++;
        // separar palabra
        char delim[] = " '\".-_`?(),!:;[]";
        char *word, *string;
        string = line;
        while ((word = strsep(&string, delim)) != NULL) { // Separar palabara por palabra.
            if (word[0] != '\0' && word[0] != '\n') {
                // convertir a lower case
                strlower(word, MAX_LINE);
                // buscar palabara en lista ligada de pregistros.
                int sptr;
                if ((sptr = listfind(word, &raiz)) >= 0) {
                    // ya esta word en la lista.
                    // leemos registros en el segundo archivo hasta encrontrar -1 
                    sregistro s;
                    fseek(sfptr, sizeof(sregistro) * sptr, SEEK_SET);
                    fread(&s, sizeof(sregistro), 1, sfptr);

                    // Cambiamos el apuntador hasta obterner el ultimo registro para esa palabra.
                    while (s.next != -1) {
                        sptr = s.next;
                        fseek(sfptr, sizeof(sregistro) * sptr, SEEK_SET);
                        fread(&s, sizeof(sregistro), 1, sfptr);
                    }

                    // Evaluar si ya se repitio la palabra en esa linea.
                    if (s.line != nline) {
                        // Tenemos sregistro de word con -1
                        sregistro nuevos;
                        nuevos.line = nline;
                        nuevos.next = -1;
                        // Actualizar registro que tenia -1;
                        s.next = ultimo_sreg + 1;
                        fseek(sfptr, sizeof(sregistro) * sptr, SEEK_SET);
                        fwrite(&s, sizeof(sregistro), 1, sfptr);
                        // Insertar nuevo registro en sregistro
                        fseek(sfptr, 0, SEEK_END);
                        ultimo_sreg++;
                        fwrite(&nuevos, sizeof(sregistro), 1, sfptr);
                    }
                } else {
                    // no esta en lista ligada de pregistos

                    // insertar nuevo sregistro al final
                    sregistro nsreg;
                    nsreg.line = nline;
                    nsreg.next = -1;
                    fseek(sfptr, 0, SEEK_END);
                    long pos = ftell(sfptr) / sizeof(sregistro);
                    fwrite(&nsreg, sizeof(sregistro), 1, sfptr);

                    // Crear un nuevo pregistro
                    pregistro *npreg;
                    npreg = (pregistro *) malloc(sizeof(pregistro));
                    strcpy(npreg->word, word);
                    npreg->ptr = ultimo_sreg + 1;
                    ultimo_sreg++;

                    // Insertamos alfabeticamente el nuevo pregistro.
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

    // Si nuestra lista ligada no leyo nada salir.
    if (raiz == NULL) {
        printf("Error al leer archivo de entrada.\n");
        return EXIT_FAILURE;
    }

    // Abrir o crear el primer archivo para escritura.
    FILE *pfptr = fopen("pregistro", "wb+");
    if (pfptr == NULL) {
        fprintf(stderr, "ERROR");
    }

    // Leer todos los registros guardados en la  lista ligada.
    // Y escribirlos en el archivo.
    pentry * curr = raiz;
    int pi = 0;
    while (curr != NULL) {
        // fwrite en pft
        pregistro p; // TODO: Lo duplicamos para sacarlo.
        strcpy(p.word, curr->p->word);
        p.ptr = curr->p->ptr;
        fwrite(&p, sizeof(pregistro), 1, pfptr);// TODO: curr (elemnto de lista) apunta a p, imprimir p, sacarlo de curr
        curr = curr->sig;
        pi++;
    }
    fclose(pfptr);

    printf("Se han creado los archivos pregistro y sregistro.\n");
    return EXIT_SUCCESS;
}
