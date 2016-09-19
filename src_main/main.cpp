/* Hello World program */

#include <stdio.h>
#include "Wiring.h"
#include "UserPinConfig.h"
// #include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


#define VOLT_DIV 5
#define MOTOR1_PIN P9_16
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
    // GPIO: LED Pin
    printf("Setup started.... \n");
    pinMode(P8_10, OUTPUT);
    setTimePeriod (MOTOR1_PIN, 20000);
    setPulseWidth (MOTOR1_PIN, 1500);
    delay(3000);

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
    printf("X: %f, Y: %f, Z: %f \n", euler.x(),euler.y(), euler.z());


    // Quaternion data
    imu::Quaternion quat = bno.getQuat();
    Serial.print("qW: %d ", quat.w());
    Serial.print("qX: %d ", quat.x());
    Serial.print("qY: %d ", quat.y());
    Serial.print("qZ: %d \n", quat.z());

    /* Display calibration status for each sensor. */
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    printf("CALIBRATION: Sys= %d, Gyro= %d, Accel= %d, Mag= %d",system,gyro,accel,mag);

    delay(BNO055_SAMPLERATE_DELAY_MS);
}













int main(void)
{
    int i=0, adc_value, motor_signal;
    float volt=0;
    setup();

    printf("Hello\n");
    
    /* start PWM */
    // analogWrite(P9_16, 60);
    // setDutyPercentage (P9_16,60);
    while(1)
    {
        adc_value = analogRead(AIN0);
        volt = 1000*adc_value/608;

        motor_signal = map(adc_value, 0, 1023, 1100,1900); // Set signal value, which should be between 1100 and 1900
        setPulseWidth (MOTOR1_PIN, motor_signal);
        // printf("ADC0: %d | ", adc_value);
        // printf("mVolts: %f\n", volt);
        imu_print();



        delay(100);
        // printf("%d",i++);
        // digitalWrite(P8_10, LOW);
        // delay(500);
        // printf("%d",i++);
        // digitalWrite(P8_10, HIGH);
    }
    return 0;
}
