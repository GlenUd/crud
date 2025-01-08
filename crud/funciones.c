#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu()
{
    int opc = 0;
    printf("1. Crear factura\n");
    printf("2. Ver facturas\n");
    printf("3. Editar factura\n");
    printf("4. Eliminar factura\n");
    printf("5. Salir\n");
    printf("Opcion: ");
    scanf("%d", &opc);
    return opc;
}

void save(struct Factura *factura)
{
    FILE *file;
    file = fopen("facturas.dat", "ab+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    else
    {
        fwrite(factura, sizeof(struct Factura), 1, file);
        printf("Factura guardada\n");
    }
    fclose(file);
}

void leerCadena(char *cadena, int numcaracteres)
{
    fflush(stdin);
    fgets(cadena, numcaracteres, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

int validarCedulaUnica(int cedula)
{
    struct Factura factura;
    FILE *file = fopen("facturas.dat", "rb");
    if (file == NULL)
        return 1; // Si no hay archivo, la cédula es única.

    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        if (factura.cedula == cedula && factura.estado == 0)
        {
            fclose(file);
            return 0; // La cédula ya existe.
        }
    }
    fclose(file);
    return 1; // La cédula es única.
}

void createFactura()
{
    struct Factura factura;
    factura.total = 0;

    printf("Ingrese la cédula del cliente: ");
    do
    {
        scanf("%d", &factura.cedula);
        if (factura.cedula <= 0)
        {
            printf("La cédula no puede ser negativa ni cero. Intente de nuevo: ");
        }
        else if (!validarCedulaUnica(factura.cedula))
        {
            printf("La cédula ya existe. Intente con otra: ");
        }
    } while (factura.cedula <= 0 || !validarCedulaUnica(factura.cedula));

    printf("Nombre del cliente: ");
    leerCadena(factura.nombre, 50);

    printf("Número de productos: ");
    do
    {
        scanf("%d", &factura.numProd);
        if (factura.numProd <= 0)
        {
            printf("El número de productos debe ser mayor que cero. Intente de nuevo: ");
        }
    } while (factura.numProd <= 0);

    for (int i = 0; i < factura.numProd; i++)
    {
        printf("Nombre del producto %d: ", i + 1);
        leerCadena(factura.productos[i].nombre, 50);

        printf("Cantidad del producto %d: ", i + 1);
        do
        {
            scanf("%d", &factura.productos[i].cantidad);
            if (factura.productos[i].cantidad <= 0)
            {
                printf("La cantidad debe ser mayor que cero. Intente de nuevo: ");
            }
        } while (factura.productos[i].cantidad <= 0);

        printf("Precio del producto %d: ", i + 1);
        do
        {
            scanf("%f", &factura.productos[i].precio);
            if (factura.productos[i].precio <= 0)
            {
                printf("El precio debe ser mayor que cero. Intente de nuevo: ");
            }
        } while (factura.productos[i].precio <= 0);

        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }

    factura.estado = 0;
    save(&factura);
}

void readFactura()
{
    struct Factura factura;

    FILE *file;
    int opc;
    file = fopen("facturas.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    printf("Cedula\t\tNombre\t\tTotal\n");
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        if (factura.estado == 0) // Solo mostrar facturas activas
        {
            printf("%d\t\t%s\t\t%.2f\n", factura.cedula,
                   factura.nombre,
                   factura.total);
        }
    }
    fclose(file);
    printf("Desea ver el detalle de alguna factura? (1. Si, 2. No): \n");
    scanf("%d", &opc);
    if (opc == 1)
    {
        printf("Ingrese la cedula del cliente: ");
        int cedula;
        scanf("%d", &cedula);
        findFacturaByCedula(cedula);
    }
}

int findFacturaByCedula(int cedula)
{
    struct Factura factura;
    int pos = -1, index = 0;
    FILE *file;
    file = fopen("facturas.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return -1;
    }
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        if (factura.cedula == cedula && factura.estado == 0)
        {
            pos = index;
            break;
        }
        index++;
    }
    fclose(file);
    return pos;
}

void updateFactura()
{
    int cedula;
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &cedula);
    int pos = findFacturaByCedula(cedula);

    if (pos != -1)
    {
        FILE *file = fopen("facturas.dat", "r+b");
        if (file == NULL)
        {
            printf("Error al abrir el archivo\n");
            return;
        }

        struct Factura factura;
        fseek(file, pos * sizeof(struct Factura), SEEK_SET);
        fread(&factura, sizeof(struct Factura), 1, file);

        printf("Nombre del cliente: ");
        leerCadena(factura.nombre, 50);

        printf("Numero de productos: ");
        scanf("%d", &factura.numProd);

        factura.total = 0;
        for (int i = 0; i < factura.numProd; i++)
        {
            printf("Producto %d:\n", i + 1);
            printf("Nombre: ");
            leerCadena(factura.productos[i].nombre, 50);
            printf("Cantidad: ");
            scanf("%d", &factura.productos[i].cantidad);
            printf("Precio: ");
            scanf("%f", &factura.productos[i].precio);

            factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
        }

        factura.estado = 0;
        fseek(file, pos * sizeof(struct Factura), SEEK_SET);
        fwrite(&factura, sizeof(struct Factura), 1, file);
        fclose(file);

        printf("Factura actualizada con éxito\n");
    }
    else
    {
        printf("Factura no encontrada\n");
    }
}

void deleteFactura()
{
    int cedula;
    printf("Ingrese la cédula del cliente: ");
    scanf("%d", &cedula);

    int pos = findFacturaByCedula(cedula);
    if (pos != -1)
    {
        FILE *file = fopen("facturas.dat", "r+b");
        if (file == NULL)
        {
            printf("Error al abrir el archivo\n");
            return;
        }

        struct Factura factura;
        fseek(file, pos * sizeof(struct Factura), SEEK_SET);
        fread(&factura, sizeof(struct Factura), 1, file);

        // Marcar la factura como eliminada
        factura.estado = 1;

        fseek(file, pos * sizeof(struct Factura), SEEK_SET);
        fwrite(&factura, sizeof(struct Factura), 1, file);
        fclose(file);

        printf("Factura con cedula %d eliminada correctamente.\n", cedula);
    }
    else
    {
        printf("Factura no encontrada para la cedula %d.\n", cedula);
    }
}