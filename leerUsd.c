#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "funciones.h"

#define BUFFER_SIZE 512
#define SECTOR_INICIAL 40000 // Nodo1:33856, Nodo2:40000
#define MAX_READ_TRY 100     // maximo numero de intentos de leer informacion de la memoria

// Para abirir la unidad de memoria micro SD
FILE *volume;
FILE *fp;
FILE *fp1;
char volume_name[7] = "\\\\.\\X:"; /*  "\\.\X:"  */

// Variable para almacenar el sector de memoria
uint32_t sector = SECTOR_INICIAL;
uint8_t cabezera = 1;
uint8_t idNodo = 0;
uint8_t running = 1;
uint8_t iteratorZero = 0;
uint16_t i = 0;
char tmp[1024];
char tmp1[1024];
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
        // Crea el archivo y guarda la informacion del primer bloque en el archivo
        if (cabezera)
        {
            if (buffer[0] != 0)
            {
                cabezera = 0;

                sprintf(tmp1, "EncabezadoNodo%02d.txt", buffer[0]);
                fp = fopen(tmp1, "at");
                if (fp == NULL)
                {
                    printf("Error al crear el archivo\n");
                }
                else
                {
                    fprintf(fp, "Nodo%02d\n", buffer[0]);
                    fprintf(fp, "Fecha: %02d/%02d/%02d\n", (uint8_t)buffer[6] - 1, (uint8_t)buffer[5], (uint8_t)buffer[7]);
                    fprintf(fp, "Hora: %02d:%02d:%02d\n", (uint8_t)buffer[3] - 5, (uint8_t)buffer[2], (uint8_t)buffer[1]);
                    uint32_t nanosegundos = (int32_t)buffer[11] << 24 | (int32_t)buffer[10] << 16 |
                                            (int32_t)buffer[9] << 8 | buffer[8]; // Nano Seconds
                    fprintf(fp, "NanoSegundos: %ld ns\n", (nanosegundos * 25));
                    sector += 1;
                }
                fclose(fp);

                sprintf(tmp, "20%02d%02d%02d%02d%02d%02dN%02d.txt", buffer[7], buffer[6] - 1, buffer[5], buffer[3] - 5, buffer[2], buffer[1], buffer[0]); //200720203960N9.txt  con aammddhhmmssNx.txt
                fp = fopen(tmp, "at");
                if (fp == NULL)
                {
                    printf("Error al crear el archivo\n");
                }
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
                if (buffer[0] == buffer[12])
                {
                    sprintf(tmp1, "FinArchivoN%02d.txt", buffer[0]);
                    fp1 = fopen(tmp1, "at");
                    if (fp1 == NULL)
                    {
                        printf("Error al crear el archivo\n");
                    }
                    else
                    {
                        fprintf(fp, "Nodo%02d\n", buffer[0]);
                        fprintf(fp, "Fecha: %02d/%02d/%02d\n", (uint8_t)buffer[6] - 1, (uint8_t)buffer[5], (uint8_t)buffer[7]);
                        fprintf(fp, "Hora: %02d:%02d:%02d\n", (uint8_t)buffer[3] - 5, (uint8_t)buffer[2], (uint8_t)buffer[1]);
                        uint32_t nanosegundos = (int32_t)buffer[11] << 24 | (int32_t)buffer[10] << 16 |
                                                (int32_t)buffer[9] << 8 | buffer[8]; // Nano Seconds
                        fprintf(fp, "NanoSegundos: %ld ns\n", (nanosegundos * 25));
                    }
                    fclose(fp);
                }
                if (buffer[511] != 0x73)
                {
                    printf("    >> Lecturan Terminada correctamente");
                    running = 0;
                    break;
                }
                int aux, fX, fY, fZ;
                aux = 0;
                aux = (int)buffer[i] << 12 | (int)buffer[i + 1] << 4 | (int)buffer[i + 2] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                fX = aux;

                aux = 0;
                aux = (int)buffer[i + 3] << 12 | (int)buffer[i + 4] << 4 | (int)buffer[i + 5] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                fY = aux;

                aux = 0;
                aux = (int)buffer[i + 6] << 12 | (int)buffer[i + 7] << 4 | (int)buffer[i + 8] >> 4;
                if ((aux & (1 << 19)) != 0)
                    aux = aux | ~((1 << 20) - 1);
                fZ = aux;

                fprintf(fp, "%10d\t%10d\t%10d\n", fX, fY, fZ);
                i += 9;
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