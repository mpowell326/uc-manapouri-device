/*!
*   main.cpp
*
*   Purpose:            Program to run on beaglebone black for controlling the tailrace inspcetion device (Nemo)
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

#include <stdio.h>
#include "Wiring.h"
#include "UserPinConfig.h"
#include "bbb_config.h"
#include "motors.h"
#include "sensors.h"

#include "tunnel_elevation.h"



/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
    // GPIO: LED Pin
    printf("Setup started.... \n");
    // motor_init(MOTOR1_PIN);
    // motor_init(MOTOR2_PIN);
    // motor_init(MOTOR3_PIN);
    // motor_init(MOTOR4_PIN);
    
    delay(3000);

    imu_init();

    lux_init();

    // ir_init();



}


int main(void)
{
    int motor_signal;

    printf("Nemo is waking up...\n");
    setup();

    
    

    while(1)
    {
        

        /* Read sensor value and map to a percantage */
        // motor_signal = map(analogRead(AIN0), 0, 1023, -100, 100 );

        /* Adjust the motor to the new speed */
        // motor_setSpeed(MOTOR1_PIN, motor_signal);
        printf("Pressure (abs): %d (Pa)", getPressure_pa(PRESSURE_SENSOR_PIN));
        imu_print();
        lux_print();
        delay(100);
    }
    return 0;
}
