/* Hello World program */

#include <stdio.h>
#include "Wiring.h"



void setup()
{
    // GPIO: LED Pin
    pinMode(P8_10, OUTPUT);
}



int main(void)
{
    int i=0, adc_value;
    float volt=0;
    setup();

    printf("Hello\n");
    
    /* start PWM */
    analogWrite(P9_16,0);
    // setDutyPercentage (P8_13,60);
    while(1)
    {
        adc_value = analogRead(AIN0);
        volt = 1000*adc_value/608;
        printf("ADC0: %d | ", adc_value);
        printf("mVolts: %d\n", volt);
        delay(500);
        // printf("%d",i++);
        // digitalWrite(P8_10, LOW);
        // delay(500);
        // printf("%d",i++);
        // digitalWrite(P8_10, HIGH);
    }
    return 0;
}
