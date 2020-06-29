#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t dataSentuSD[512] = {0};
uint8_t dataAdxl[63] = {0};
uint32_t sector = 35000;
int16_t i = 0;
bool running = true;
void llenar(void);
void llenardata(int8_t value);
uint8_t iterar = 1;

int main()
{
    int32_t value = (int32_t)iterar << 16;
    printf("%ld\n", value);
    //int8_t contar = 0;

    //llenardata(2);
    //while (running)
    //{
      //  llenar();
    //}
    return 0;
}

void llenardata(int8_t value)
{
    for (i = 0; i < 63; i++)
    
    {
        dataAdxl[i] = value;
    }
    i = 0;
}

void llenar(void)
{
    uint8_t j = 0;
    uint8_t k = 0;
    for (j = 10; j < 63; j++)
    {
        if (i < 504)
        {
            dataSentuSD[i] = dataAdxl[j];
            i++;

            if (i == 504)
            {
                for (k = 0; k < 8; k++)
                {
                    dataSentuSD[i] = 0;
                    i++;
                }
                for (i = 0; i < 512; i++)
                {
                    printf("%03d %02d\n", i, dataSentuSD[i]);
                }

                running = false;
            }
        }
    }
}
