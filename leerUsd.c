#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "funciones.h"

#define BUFFER_SIZE 512
#define SECTOR_INICIAL 35000
#define MAX_READ_TRY 100 // maximo numero de intentos de leer informacion de la memoria

// Para abirir la unidad de memoria micro SD
FILE *volume;
FILE *fp;
char volume_name[7] = "\\\\.\\X:"; /*  "\\.\X:"  */

// Variable para almacenar el sector de memoria
unsigned long sector = SECTOR_INICIAL;
uint8_t cabezera = 1;
uint8_t idNodo = 0;
uint8_t running = 1;
uint8_t i = 0;
char tmp[1024];
// Buffer para almacenar el contenido de cada sector de memoria
uint8_t buffer[BUFFER_SIZE] = {0};

int main(void)
{
    TextoInicial();
    scanf("%c", &volume_name[4]); // se cambia la 'X'(volume_name[4]) por la letra correspondiente a la micro SD
    fflushstdin();
    volume = fopen(volume_name, "r");
    setbuf(volume, NULL); // Desactiva buffering
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
    }
    // Bucle de generación de archivos
    while (running)
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
        if (cabezera)
        {
            if (buffer[0] != 0)
            {
                cabezera = 0;
                sprintf(tmp, "DatosNod%d.txt", buffer[0]);
                fp = fopen(tmp, "a");
                fprintf(fp, "Fecha: %02d/%02d/%02d\n", buffer[5], buffer[6], buffer[7]);
                fprintf(fp, "Hora: %02d:%02d:%02d\n", buffer[3], buffer[2], buffer[1]);
                uint32_t nanosegundos = (int32_t)buffer[11] << 24 | (int32_t)buffer[10] << 16 |
                                        (int32_t)buffer[9] << 8 | buffer[8]; // Nano Seconds
                fprintf(fp, "Nano Segundos: %ld\n\n\n\n", nanosegundos);
                fprintf(fp, "%10s%10s%10s\n", "Eje X", "Eje Y", "Eje Z");
                sector++;
            }
            else
            {
                printf("    >> No existe datos para leer\n");
                running = 0;
            }
        }
        else
        {
            i = 0;
            while (i < 496)
            {
                int aux, fX, fY, fZ;
                aux = 0;
                aux = (int)buffer[i] << 12 | (int)buffer[i + 1] << 4 | (int)buffer[i + 2] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                fX = aux;
                //fX = aux / 256000.00;

                aux = 0;
                aux = (int)buffer[i + 3] << 12 | (int)buffer[i + 4] << 4 | (int)buffer[i + 5] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                fY = aux;
                //fY = aux / 256000.00;

                aux = 0;
                aux = (int)buffer[i + 6] << 12 | (int)buffer[i + 7] << 4 | (int)buffer[i + 8] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                //fZ = aux / 256000.00;
                fZ = aux;
                if (fX == 0 && fY == 0 && fZ == 0)
                {
                    printf("    >> Lecturan Terminada correctamente");
                    running = 0;
                }
                else
                {
                    fprintf(fp, "%10d\t%10d\t%10d\n", fX, fY, fZ);
                    i += 9;
                }
            }
            sector++;
        }
    }
    fclose(fp);
    fclose(volume);
    Sleep(1000);
    return 0;
} // end main

/* End File */