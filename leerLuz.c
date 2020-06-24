#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "funciones.h"

#define BUFFER_SIZE 512
#define SECTOR_INICIAL 2051
#define MAX_READ_TRY 100 // maximo numero de intentos de leer informacion de la memoria

// Para abirir la unidad de memoria micro SD
FILE *volume;
FILE *fp;
char volume_name[7] = "\\\\.\\X:"; /*  "\\.\X:"  */

// Variable para almacenar el sector de memoria
unsigned long sector = SECTOR_INICIAL;
unsigned char valor1;
unsigned char valor2;

// Buffer para almacenar el contenido de cada sector de memoria
unsigned char buffer[BUFFER_SIZE] = {0};

int main(void)
{
    unsigned char sd = 1;
    int contador = 1, i = 0;
    valor1 = 0;
    valor2 = 0;
    TextoInicial();
    scanf("%c", &volume_name[4]); // se cambia la 'X'(volume_name[4]) por la letra correspondiente a la micro SD
    fflushstdin();

    // Se abre la unidad de memoria micro SD en modo lectura "r"
    volume = fopen(volume_name, "r");
    setbuf(volume, NULL); // Desactiva buffering

    fp = fopen("fichero.txt", "a");
    fprintf(fp, "%10s%10s\n", "Eje V1", "Eje V2");

    // Check de la unidad
    if (!volume)
    {
        printf("    >> No se puede abrir la unidad especificada\n");
        Sleep(2000);
        return -1;
    }
    else
    {
        printf("    >> Memoria seleccionada correctamente\n");
        printf("    >> Sector a leer N %d, espere...\n", sector);
    }

    // Bucle de generación de archivos
    while (1)
    {
        if (fseek(volume, sector * BUFFER_SIZE, SEEK_SET) != 0)
        {
            // Salir si no se puede apuntar al sector de memoria especificado
            printf("No se pudo apuntar al sector %d\n", sector);
            printf("%ld", ftell(volume));
            break;
        }

        // Lee en 'buffer' el contenido del sector de memoria
        fread(buffer, sizeof(*buffer), BUFFER_SIZE, volume);

        i = 0;
        while (i < 511)
        {
            valor1 = buffer[i];
            valor2 = buffer[i + 1];
            fprintf(fp, "%10d%10d\n", valor1, valor2);
            i += 2;
        }
        break;
    }
    printf(" >> Se lee hasta el sector %d\n", sector);
    fclose(fp);
    fclose(volume);
    Sleep(1000);
    return 0;
}

/*
 * End File 
 */