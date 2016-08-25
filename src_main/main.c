/* Hello World program */

#include <stdio.h>
#include "Wiring.h"



void setup()
{
    // GPIO: LED Pin
    pinMode(P8_10, OUTPUT);

    // PWM
    pinMode(P8_13);
}



int main(void)
{
    int i=0;

    setup();

    printf("Hello\n");
    
    //  start PWM
    analogWrite(P8_13,230);

    while(1)
    {
        delay(500);
        printf("%d",i++);
        digitalWrite(P8_10, LOW);
        delay(500);
        printf("%d",i++);
        digitalWrite(P8_10, HIGH);
    }
    return 0;
}
