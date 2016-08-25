/* Hello World program */

#include <stdio.h>
#include "Wiring.h"



void setup()
{
    // GPIO: LED Pin
    pinMode(P8_10, OUTPUT);

    // PWM
    //pinMode(P8_13,OUTPUT);
}



int main(void)
{
    int i=0, adc_value;

    setup();

    printf("Hello\n");
    
    //  start PWM
    // analogWrite(P9_27,50);
    // setDutyPercentage (P8_13,60);
    while(1)
    {
        adc_value = analogRead(AIN0);
        printf("%d\n", adc_value);
        delay(500);
        // printf("%d",i++);
        // digitalWrite(P8_10, LOW);
        // delay(500);
        // printf("%d",i++);
        // digitalWrite(P8_10, HIGH);
    }
    return 0;
}
