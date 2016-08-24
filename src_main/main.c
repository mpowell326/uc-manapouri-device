/* Hello World program */

#include <stdio.h>
#include "Wiring.h"



void setup()
{
    pinMode(8_10, OUTPUT);
}



main(void)
{
    pinMode(P8_10, OUTPUT);
    printf("Hello World !!\n");

     digitalWrite(8_10, HIGH);
    return 0;
}
