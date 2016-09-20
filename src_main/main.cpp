/* Hello World program */

#include <stdio.h>
#include "Wiring.h"
#include "UserPinConfig.h"
#include "bbb_config.h"
#include "motors.h"
#include "sensors.h"




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

    // imu_init();

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
        // motor_signal = map(analogRead(AIN0), -100, 100, 0, 1023 );

        /* Adjust the motor to the new speed */
        // motor_setSpeed(MOTOR1_PIN, motor_signal);

        // imu_print();
        lux_print();
        delay(100);
    }
    return 0;
}
