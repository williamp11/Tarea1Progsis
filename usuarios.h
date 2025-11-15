#ifndef USUARIOS_H
#define USUARIOS_H
#include <stdbool.h>

// Estructura para los usuarios
typedef struct {
    char usuario[20];
    char clave[20];
    char nombre[50];
} Vendedor;

// Declaración de funciones
void cargar_vendedores(Vendedor **lista, int *count);
bool verificar_credenciales(Vendedor *lista, int count, const char *user, const char *pass);

#endif