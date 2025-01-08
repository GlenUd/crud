#include <string.h>

struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
};
struct Factura {
    char nombre[50];
    int cedula;
    int numprod;
    struct Producto productos[50];
    float total;
    int estado;
};

int menu();
void save(struct Factura *factura);
void leercadena(char *cadena, int numcaracteres);
void createFactura();
void readFactura();
int findfacturabycedula(int cedula);
void updatefactura(int posicion);
void deletefactura(int posicion);
