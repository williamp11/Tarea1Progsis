#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "usuarios.h"
#include "productos.h"

typedef struct DetalleVenta {
    Producto *producto;
    int cantidad;
    struct DetalleVenta *next;
} DetalleVenta;

Vendedor *vendedores = NULL;
int num_vendedores = 0;
Producto *productos = NULL;
int num_productos = 0;

void obtener_fecha_actual(char *fecha) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(fecha, 11, "%Y-%m-%d", tm);
}

void autenticar_usuario() {
    char user[20], pass[20];
    int intentos = 0;
    
    do {
        printf(" AUTENTICACION \n");
        printf("Usuario: ");
        scanf("%19s", user);
        printf("Clave: ");
        scanf("%19s", pass);
        
        if (verificar_credenciales(vendedores, num_vendedores, user, pass)) {
            printf("\nBienvenido!\n");
            return;
        } else {
            intentos++;
            printf("Usuario o clave incorrectos. Intentos restantes: %d\n\n", 3 - intentos);
        }
    } while (intentos < 3);

    printf("\n Maximo de intentos alcanzado\n");
    exit(1);
}

void liberar_detalle(DetalleVenta *head) {
    DetalleVenta *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void registrar_ventas_flujo() {
    char continuar[5] = "si";
    char codigo[10];
    int cantidad_vender;
    DetalleVenta *head = NULL;
    DetalleVenta *current = NULL;
    float total_venta = 0.0;
    
    int num_factura = obtener_ultimo_num_factura() + 1;
    char fecha_actual[11];
    obtener_fecha_actual(fecha_actual);
    
    printf("\n REGISTRO DE VENTA \n");
    printf("Fecha: %s | Numero de Factura: %d\n", fecha_actual, num_factura);
    printf("---------------------------\n");

    while (tolower(continuar[0]) == 's') {
        printf("Codigo de Producto a vender (o 0 para terminar): ");
        scanf("%9s", codigo);
        
        if (strcmp(codigo, "0") == 0) break;

        Producto *p = buscar_producto(productos, num_productos, codigo);

        if (p == NULL) {
            printf("Codigo de producto no encontrado.\n");
            continue;
        }
        
        printf("Producto: %s. Cantidad disponible: %d. Precio: %.2f\n", 
               p->nombre, p->cantidad_disponible, p->precio_venta);
               
        printf("Cantidad a vender: ");
        if (scanf("%d", &cantidad_vender) != 1 || cantidad_vender <= 0) {
            printf("Cantidad invalida.\n");
            continue;
        }

        if (cantidad_vender > p->cantidad_disponible) {
            printf("Solo quedan %d unidades disponibles.\n", p->cantidad_disponible);
            continue;
        }

        DetalleVenta *new_detalle = (DetalleVenta*)malloc(sizeof(DetalleVenta));
        if (new_detalle == NULL) {
            fprintf(stderr, "Error al asignar memoria para detalle de venta.\n");
            break;
        }
        new_detalle->producto = p;
        new_detalle->cantidad = cantidad_vender;
        new_detalle->next = NULL;

        if (head == NULL) {
            head = new_detalle;
            current = head;
        } else {
            current->next = new_detalle;
            current = new_detalle;
        }
        
        total_venta += (float)cantidad_vender * p->precio_venta;
        
        printf("Producto agregado. ¿Desea registrar otro producto? (si/no): ");
        scanf("%4s", continuar);
    }

    printf("\n DETALLE DE LA VENTA %d \n", num_factura);
    printf("Producto | Cantidad | Precio Unitario | Total Item\n");
    printf("--------------------------------------------------\n");
    
    DetalleVenta *temp = head;
    while(temp != NULL) {
        float total_item = (float)temp->cantidad * temp->producto->precio_venta;
        printf("%s | %d | %.2f | %.2f\n", 
               temp->producto->nombre, 
               temp->cantidad, 
               temp->producto->precio_venta, 
               total_item);
        temp = temp->next;
    }
    printf("--------------------------------------------------\n");
    printf("TOTAL VENTA: %.2f\n", total_venta);

    char confirmacion[5];
    printf("\nConfirma la venta? (si/no): ");
    scanf("%4s", confirmacion);

    if (tolower(confirmacion[0]) == 's') {
        DetalleVenta *venta_final = head;
        while(venta_final != NULL) {
            venta_final->producto->cantidad_disponible -= venta_final->cantidad;
            
            Venta v;
            v.num_factura = num_factura;
            strcpy(v.codigo_producto, venta_final->producto->codigo);
            strcpy(v.nombre_producto, venta_final->producto->nombre);
            v.cantidad_vendida = venta_final->cantidad;
            v.costo = venta_final->producto->costo;
            v.precio_venta = venta_final->producto->precio_venta;
            strcpy(v.fecha_venta, fecha_actual);
            
            registrar_venta(v);
            
            venta_final = venta_final->next;
        }
        
        guardar_inventario(productos, num_productos);
        printf("\nVENTA %d REGISTRADA EXITOSAMENTE.\n", num_factura);
    } else {
        printf("\nVenta cancelada.\n");
    }

    liberar_detalle(head);
}


int main() {
    cargar_vendedores(&vendedores, &num_vendedores);
    cargar_productos(&productos, &num_productos);
    
    autenticar_usuario();
    
    registrar_ventas_flujo();
    
    free(vendedores);
    free(productos);
    
    return 0;
}