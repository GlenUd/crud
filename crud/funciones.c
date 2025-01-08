#include <stdio.h>
#include "funciones.h"

int menu(){
    int opc;
    printf("1. Crear factura\n");
    printf("2. Leer facturas\n");
    printf("3. Buscar factura por cedula\n");
    printf("4. Actualizar factura\n");
    printf("5. Eliminar factura\n");
    printf("6. Salir\n");
    printf("Opcion: ");
    scanf("%d", &opc);
    return opc;
}
void save(struct Factura *factura){
    FILE *file;
    file = fopen("factura.dat", "ab+");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }else{
        fwrite(factura, sizeof(struct Factura), 1, file);
        printf("Factura guardada\n");
    }
    fclose(file);
}

void leercadena(char *cadena, int numcaracteres){
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Clear the input buffer
    fgets(cadena, numcaracteres, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

void createFactura(){
    struct Factura factura;
    factura.total = 0;
    printf("Nombre del cliente: ");
    leercadena(factura.nombre, 50);
    printf("Cedula del cliente: ");
    scanf("%d", &factura.cedula);
    printf("Numero de productos: ");
    scanf("%d", &factura.numprod);
    for(int i = 0; i < factura.numprod; i++){
        printf("Nombre del producto %d: ", i + 1);
        leercadena(factura.productos[i].nombre, 50);
        printf("Cantidad: ");
        scanf("%d", &factura.productos[i].cantidad);
        printf("Precio: ");
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].precio * factura.productos[i].cantidad;
    }
    factura.estado = 0;
    save(&factura);
}

void readFactura(){
    FILE *file;
    struct Factura factura;
    file = fopen("factura.dat", "rb");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }
    printf("cedula\t\tNombre\t\tTotal\n");
    while(fread(&factura, sizeof(struct Factura), 1, file)){
        if(factura.estado != 2) { // Check if the factura is not deleted
            printf("%d\t\t%s\t\t%.2f\n",    factura.cedula,
                                            factura.nombre, 
                                            factura.total);
        }
    }
    fclose(file);
}

int findfacturabycedula(int cedula){
    struct Factura factura;
    int f = 0, posicion = -1;
    FILE *file;
    file = fopen("factura.dat", "rb");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return -1;
    }
    while(fread(&factura, sizeof(struct Factura), 1, file)){
        if(factura.cedula == cedula && factura.estado != 2){
            f = 1;
            printf("factura encontrada\n");
            printf("Nombre: %s\n", factura.nombre);
            printf("Cedula: %d\n", factura.cedula);
            printf("Total: %.2f\n", factura.total);
            printf("Productos\n");
            printf("Nombre\t\tCantidad\t\tPrecio\n");           
            for(int i = 0; i < factura.numprod; i++){
                printf("%s\t\t%d\t\t%.2f\n", factura.productos[i].nombre,
                                            factura.productos[i].cantidad,
                                            factura.productos[i].precio);
            }
            printf("total: %.2f\n", factura.total);
            posicion = ftell(file) - sizeof(struct Factura); // Correct position
            break;
        }
    }

    if (f == 0){
        printf("Factura no encontrada\n");
    }
    fclose(file);
    return posicion;
}

void updatefactura(int posicion) {
    struct Factura factura;
    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    fseek(file, posicion, SEEK_SET);
    fread(&factura, sizeof(struct Factura), 1, file);

    printf("Actualizar datos de la factura:\n");
    printf("Nombre del cliente (%s): ", factura.nombre);
    leercadena(factura.nombre, 50);
    printf("Cedula del cliente (%d): ", factura.cedula);
    scanf("%d", &factura.cedula);
    printf("Numero de productos (%d): ", factura.numprod);
    scanf("%d", &factura.numprod);
    factura.total = 0;
    for (int i = 0; i < factura.numprod; i++) {
        printf("Nombre del producto %d (%s): ", i + 1, factura.productos[i].nombre);
        leercadena(factura.productos[i].nombre, 50);
        printf("Cantidad (%d): ", factura.productos[i].cantidad);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Precio (%.2f): ", factura.productos[i].precio);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].precio * factura.productos[i].cantidad;
    }

    fseek(file, posicion, SEEK_SET);
    fwrite(&factura, sizeof(struct Factura), 1, file);
    fclose(file);
    printf("Factura actualizada\n");
}


void deletefactura(int posicion){
    struct Factura factura;
    FILE *file;
    file = fopen("factura.dat", "rb+");
    if(file == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }
    fseek(file, posicion, SEEK_SET);
    fread(&factura, sizeof(struct Factura), 1, file);
    factura.estado = 2;
    fseek(file, posicion, SEEK_SET);
    fwrite(&factura, sizeof(struct Factura), 1, file);
    fclose(file);
}