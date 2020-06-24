#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "funciones.h"

#define BUFFER_SIZE 512
#define SECTOR_INICIAL 32953
#define MAX_READ_TRY 100 // maximo numero de intentos de leer informacion de la memoria

// Para abirir la unidad de memoria micro SD
FILE *volume;
FILE *fp;
char volume_name[7] = "\\\\.\\X:"; /*  "\\.\X:"  */

// Variable para almacenar el sector de memoria
unsigned long sector = SECTOR_INICIAL;
long x0, x1, x2, x3;
long valorX;
long valorY;
long valorZ;
float fvX, fvY, fvZ;

// Buffer para almacenar el contenido de cada sector de memoria
unsigned char buffer[BUFFER_SIZE] = {0};

int main(void)
{
    unsigned char sd = 1;
    int contador = 1, i = 0;
    valorX = 1;
    valorY = 1;
    valorZ = 1;
    TextoInicial();
    scanf("%c", &volume_name[4]); // se cambia la 'X'(volume_name[4]) por la letra correspondiente a la micro SD
    fflushstdin();

    // Se abre la unidad de memoria micro SD en modo lectura "r"
    volume = fopen(volume_name, "r");
    setbuf(volume, NULL); // Desactiva buffering

    fp = fopen("fichero.txt", "a");
    fprintf(fp, "%10s%10s%10s\n", "Eje X", "Eje Y", "Eje Z");

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
        while (i < 504)
        {
            if (valorX == 0 && valorY == 0 && valorZ == 0)
            {
                sd = 0;
                break;
            }
            x3 = buffer[i];
            x3 <<= 12;
            x2 = buffer[i + 1];
            x2 <<= 4;
            x1 = buffer[i + 2];
            x1 >>= 4;
            valorX = x3 | x2 | x1;
            if ((valorX & 0x80000) == 0x80000)
            {
                valorX = ~(valorX) & 0x00FFFFF;
                valorX = ~(valorX);
            }
            fvX = valorX / 256000.00;
            x3 = buffer[i + 3];
            x3 <<= 12;
            x2 = buffer[i + 4];
            x2 <<= 4;
            x1 = buffer[i + 5];
            x1 >>= 4;
            valorY = x3 | x2 | x1;
            if ((valorY & 0x80000) == 0x80000)
            {
                valorY = ~(valorY)&0x0FFFFF;
                valorY = ~(valorY);
            }
            fvZ = valorZ / 256000.00;
            x3 = buffer[i + 6];
            x3 <<= 12;
            x2 = buffer[i + 7];
            x2 <<= 4;
            x1 = buffer[i + 8];
            x1 >>= 4;
            valorZ = x3 | x2 | x1;
            if ((valorZ & 0x80000) == 0x80000)
            {
                valorZ = ~(valorZ)&0x0FFFFF;
                valorZ = ~(valorZ);
            }
            fvY = valorY / 256000.00;
            //printf("%3.3f %3.3f %3.3f\n", fvX,fvY,fvZ);
            fprintf(fp, "%10f%10f%10f\n", fvX, fvY, fvZ);
            i += 9;
        }
        if (sd == 0)
            break;
        sector++;
        //break;
    }
    printf(" >> Se lee hasta el sector %d\n", sector);
    fclose(fp);
    fclose(volume);
    Sleep(1000);
    return 0;
} // end main


/* End File */