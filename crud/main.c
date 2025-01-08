#include <stdio.h>
#include "funciones.h"

int main(){
    int opc= 0, opc2;
    do{
        opc = menu();
        switch(opc){
            case 1:
                createFactura();
                break;
            case 2:
                readFactura();
                break;
            case 3:
                int cedula;
                printf("Ingrese la cedula: ");
                scanf("%d", &cedula);
                findfacturabycedula(cedula);
                break;
            case 4:
                int posicion;
                printf("Ingrese la cedula: ");
                scanf("%d", &cedula);
                posicion = findfacturabycedula(cedula);
                updatefactura(posicion);
                break;
            case 5:
                printf("Ingrese la cedula: ");
                scanf("%d", &cedula);
                posicion = findfacturabycedula(cedula);
                deletefactura(posicion);
                break;
            case 6:
                printf("Adios\n");
                break;
            default:
                printf("Opcion no valida\n");
                break;

        }
        printf("desea otra opcion? 1. si 2. no\n");
        scanf("%d", &opc2);
    }while(opc2 == 1);
    return 0;
}