#include "usuarios.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cargar_vendedores(Vendedor **lista, int *count) {
    FILE *file = fopen("vendedores.txt", "r");
    if (!file) {
        fprintf(stderr, "Error al encontrar el archivo vendedores.txt\n");
        exit(1);
    }
    
    int lines = 0;
    char ch;
    while(!feof(file)) {
      ch = fgetc(file);
      if(ch == '\n') lines++;
    }
    rewind(file);

    *lista = (Vendedor*)malloc(sizeof(Vendedor) * lines);
    if (*lista == NULL) {
        fprintf(stderr, "Error al asignar memoria para vendedores.\n");
        exit(1);
    }

    *count = 0;
    while(*count < lines && 
          fscanf(file, "%19[^,],%19[^,],%49[^\n]\n", 
                 (*lista)[*count].usuario, 
                 (*lista)[*count].clave, 
                 (*lista)[*count].nombre) == 3) {
        (*count)++;
    }
    fclose(file);
}

bool verificar_credenciales(Vendedor *lista, int count, const char *user, const char *pass) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].usuario, user) == 0 && strcmp(lista[i].clave, pass) == 0) {
            return true;
        }
    }
    return false;
}