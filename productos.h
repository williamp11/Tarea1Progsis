#ifndef PRODUCTOS_H
#define PRODUCTOS_H
 
// Estructura para el inventario
typedef struct {
    char codigo[10];
    char nombre[50];
    int cantidad_disponible;
    float costo;         
    float precio_venta;
} Producto;
 
// Estructura para registrar una venta
typedef struct {
    int num_factura;
    char codigo_producto[10];
    char nombre_producto[50];
    int cantidad_vendida;
    float costo;
    float precio_venta;
    char fecha_venta[11]; //aaaa-mm-dd
} Venta;
 
// Declaración de funciones
void cargar_productos(Producto **lista, int *count);
void guardar_inventario(Producto *lista, int count);
Producto* buscar_producto(Producto *lista, int count, const char *codigo);
int obtener_ultimo_num_factura();
void registrar_venta(Venta venta_a_registrar);

#endif