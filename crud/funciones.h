struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura {
    char nombre[50];
    int cedula;
    int numProd;
    struct Producto productos[5];
    float total;
    int estado;
};

int menu();
void readFactura();
void createFactura();
void save(struct Factura *factura);
void leerCadena(char *cadena, int numcaracteres);
int findFacturaByCedula(int cedula);
void updateFactura();
void deleteFactura();