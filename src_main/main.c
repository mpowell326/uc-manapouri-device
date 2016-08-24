/* Hello World program */

#include <stdio.h>
#include "Wiring.h"



void setup()
{
    pinMode(8_10, OUTPUT);
}



void main(void)
{
    int i=0;
    printf("Hello World !!\n");
    while(1)
    {
        printf("%d",i++);
        digitalWrite(8_10, HIGH);
    }
}
