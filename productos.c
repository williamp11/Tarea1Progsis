#include "productos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cargar_productos(Producto **lista, int *count) {
    FILE *file = fopen("producto.txt", "r");
    if (!file) {
        fprintf(stderr, "Error al encontrar el archivo producto.txt\n");
        exit(1);
    }
    
    int lines = 0;
    char ch;
    while(!feof(file)) {
      ch = fgetc(file);
      if(ch == '\n') lines++;
    }
    rewind(file);

    *lista = (Producto*)malloc(sizeof(Producto) * lines);
    if (*lista == NULL) {
        fprintf(stderr, "Error al asignar memoria para productos.\n");
        exit(1);
    }

    *count = 0;
    while(*count < lines && 
          fscanf(file, "%9[^,],%49[^,],%d,%f,%f\n", 
                 (*lista)[*count].codigo, 
                 (*lista)[*count].nombre, 
                 &(*lista)[*count].cantidad_disponible, 
                 &(*lista)[*count].costo, 
                 &(*lista)[*count].precio_venta) == 5) {
        (*count)++;
    }
    fclose(file);
}

void guardar_inventario(Producto *lista, int count) {
    FILE *file = fopen("producto.txt", "w");
    if (!file) {
        fprintf(stderr, "Error al abrir producto.txt para guardar.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%d,%.2f,%.2f\n", 
                lista[i].codigo, 
                lista[i].nombre, 
                lista[i].cantidad_disponible, 
                lista[i].costo, 
                lista[i].precio_venta);
    }
    fclose(file);
}

Producto* buscar_producto(Producto *lista, int count, const char *codigo) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].codigo, codigo) == 0) {
            return &lista[i];
        }
    }
    return NULL;
}

int obtener_ultimo_num_factura() {
    FILE *file = fopen("ventas.txt", "r");
    if (!file) return 0;

    int last_num = 0;
    char line[256];
    
    while(fgets(line, sizeof(line), file) != NULL) {
        int current_num;
        if (sscanf(line, "%d,", &current_num) == 1) {
            last_num = current_num;
        }
    }
    
    fclose(file);
    return last_num;
}

void registrar_venta(Venta venta_a_registrar) {
    FILE *file = fopen("ventas.txt", "a");
    if (!file) {
        fprintf(stderr, "Error al abrir ventas.txt para registrar la venta.\n");
        return;
    }

    fprintf(file, "%d,%s,%s,%d,%.2f,%.2f,%s\n",
            venta_a_registrar.num_factura,
            venta_a_registrar.codigo_producto,
            venta_a_registrar.nombre_producto,
            venta_a_registrar.cantidad_vendida,
            venta_a_registrar.costo,
            venta_a_registrar.precio_venta,
            venta_a_registrar.fecha_venta);
            
    fclose(file);
}