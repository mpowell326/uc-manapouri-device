/* Hello World program */

#include <stdio.h>
#include "Wiring.h"
#include "UserPinConfig.h"
#include "bbb_config.h"
#include "motors.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


#define VOLT_DIV 5
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
    // GPIO: LED Pin
    printf("Setup started.... \n");
    motor_init(MOTOR1_PIN);
    motor_init(MOTOR2_PIN);
    motor_init(MOTOR3_PIN);
    motor_init(MOTOR4_PIN);
    
    delay(3000);

    // imu_init();

    // lux_init();

    // ir_init();


    printf("Orientation Sensor Raw Data Test \n");
    /* Initialise the sensor */
    if(!bno.begin())
    {
    /* There was a problem detecting the BNO055 ... check your connections */
        printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\n");
        while(1);
    }

    delay(1000);

    /* Display the current temperature */
    int8_t temp = bno.getTemp();
    printf("Current Temperature: %d C\n",temp);

    bno.setExtCrystalUse(true);

    printf("Calibration status values: 0=uncalibrated, 3=fully calibrated\n");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void imu_print(void)
{
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    /* Display the floating point data */
    printf("X: %f, Y: %f, Z: %f     |   ", euler.x(),euler.y(), euler.z());


    // Quaternion data
    imu::Quaternion quat = bno.getQuat();
    printf("qW: %f ", (float)quat.w());
    printf("qX: %f ", (float)quat.x());
    printf("qY: %f ", (float)quat.y());
    printf("qZ: %f  |   ", (float)quat.z());

    /* Display calibration status for each sensor. */
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    printf("CALIBRATION: Sys= %d, Gyro= %d, Accel= %d, Mag= %d \n",system,gyro,accel,mag);

    delay(BNO055_SAMPLERATE_DELAY_MS);
}













int main(void)
{
    int motor_signal;

    printf("Nemo is waking up...\n");
    setup();

    
    

    while(1)
    {

        /* Read sensor value and map to a percantage */
        motor_signal = map(analogRead(AIN0), -100, 100, 0, 1023 );

        /* Adjust the motor to the new speed */
        motor_setSpeed(MOTOR1_PIN, motor_signal);

        imu_print();
        int8_t temp = bno.getTemp();
        printf("Current Temperature: %d C\n",temp);

        delay(100);
    }
    return 0;
}
