#include <stdio.h>

void fflushstdin(void)
{
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n')
        ;
}

void TextoInicial(void)
{
    printf("  -------------------------------------------------------\n");
    printf("                   LECTOR uSD\n");
    printf("                 LEER DATOS DE uSD\n");
    printf("  -------------------------------------------------------\n");
    printf("    Ingrese la letra de la unidad de memoria micro SD.\n");
    printf("    >> Unidad: ");
}

char *month_name(int n)
{
    static char *name[] = {
        "unk",
        "Ene",
        "Feb",
        "Mar",
        "Abr",
        "May",
        "Jun",
        "Jul",
        "Ago",
        "Sep",
        "Oct",
        "Nov",
        "Dic"};
    return (n < 1 || n > 12) ? name[0] : name[n];
}

// Funcion para incrementar en 1 un numero BCD
unsigned char IncrementBCD(unsigned char n)
{
    unsigned char byteH, byteL;
    if (n > 99)
        n = 0;
    byteH = n >> 4;
    byteL = n & 0x0F;
    byteL++;
    if (byteL > 9)
    {
        byteL = 0;
        byteH++;
        if (byteH > 9)
            byteH = 0;
    }
    return ((byteH << 4) | byteL);
}

/**
 * End File
 */