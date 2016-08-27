#include "motors.h"

#define freq_hz 50;

void Motormove (int motorpin, int PWMpercentage, int time){



    setTimePeriod (motorpin, uint32_t period_us);
    setFrequency (motorpin, freq_hz);
    setPulseWidth (motorpin, uint32_t period_us);
    setDutyPercentage (motorpin, uint32_t percentage);



    // length of time the motor is running for
    delay(time)

    // returning the motor to 1500hz
    analogWrite(motorpin, uint8_t value)


}
