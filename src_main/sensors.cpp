/*!
*   \sensors.cpp
*
*   Purpose:            Module to read various sensor values
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/

#include "sensors.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)


// If gain = false (0), device is set to low gain (1X)
// If gain = high (1), device is set to high gain (16X)
#define LUX_GAIN    0

// If time = 0, integration will be 13.7ms
// If time = 1, integration will be 101ms
// If time = 2, integration will be 402ms
// If time = 3, use manual start / stop to perform your own integration
#define LUX_TIME    2



#define PSI_TO_PASCAL       6894.76 //psi
#define PASCAL_TO_ATM       101325  //Pascal
#define PASCAL_TO_BAR       100000  //Pascal
#define PASCAL_TO_MH2O     0.0001019744289  
/*-----------------------------------------------------------------------------------------------*/
/* Macros                                                                                        */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/* Types                                                                                         */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/* Function Prototypes                                                                           */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/* Constants                                                                                     */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/* Variables                                                                                     */
/*-----------------------------------------------------------------------------------------------*/

// Create a sensor object for the IMU:
Adafruit_BNO055 bno_sensor = Adafruit_BNO055();


// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;
// Integration ("shutter") time in milliseconds
unsigned int ms;

/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/
/* Implementation                                                                                */
/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/

/*-----------------------------------------------------------------------------------------------*/
/* Private Functions:                                                                            */
/*-----------------------------------------------------------------------------------------------*/

// If there's an I2C error, this function will
// print out an explanation.
void lux_printError(byte error)
{
    printf("I2C error: %d, ", error);

    switch(error)
    {
        case 0:
            printf("success\n");
            break;
        case 1:
            printf("data too long for transmit buffer\n");
            break;
        case 2:
            printf("received NACK on address (disconnected?)\n");
            break;
        case 3:
            printf("received NACK on data\n");
            break;
        case 4:
            printf("other error\n");
            break;
        default:
            printf("unknown error\n");
    }
}
/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/
void lux_init()
{
    printf("Initialsing Lux Sensor ...\n\n");

    light.begin();


    // The light sensor has a default integration time of 402ms,
    // and a default gain of low (1X).

    // If you would like to change either of these, you can
    // do so using the setTiming() command.

    // setTiming() will set the third parameter (ms) to the
    // requested integration time in ms (this will be useful later):

    printf("Set timing...\n");
    light.setTiming(LUX_GAIN,LUX_TIME,ms);

    // To start taking measurements, power up the sensor:

    printf("Powerup...\n");
    light.setPowerUp();

    // The sensor will now gather light during the integration time.
    // After the specified time, you can retrieve the result from the sensor.
    // Once a measurement occurs, another integration period will start.
}


void imu_init()
{
    printf("Initialsing IMU Sensor ...\n \n");
    /* Initialise the sensor */
    if(!bno_sensor.begin())
    {
    /* There was a problem detecting the BNO055 ... check your connections */
        printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\n");
        while(1);
    }

    delay(1000);

    /* Display the current temperature */
    int8_t temp = bno_sensor.getTemp();
    printf("Current Temperature: %d C\n",temp);

    bno_sensor.setExtCrystalUse(true);

    printf("Calibration status values: 0=uncalibrated, 3=fully calibrated\n");
}

double getLux()
{
    unsigned int data0, data1;
    double lux = 0;    // Resulting lux value

    if (light.getData(data0,data1))
    {
        // getData() returned true, communication was successful

        printf("data0: %d, data1: %d    | ", data0, data1);


        // To calculate lux, pass all your settings and readings
        // to the getLux() function.

        // The getLux() function will return 1 if the calculation
        // was successful, or 0 if one or both of the sensors was
        // saturated (too much light). If this happens, you can
        // reduce the integration time and/or gain.
        // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

        boolean good;  // True if neither sensor is saturated

        // Perform lux calculation:

        good = light.getLux(LUX_GAIN,ms,data0,data1,lux);


        printf("lux: %f ", lux);
        if (good) printf(" (good)\n"); else printf(" (BAD)\n");
    }
    else
    {
        // getData() returned false because of an I2C error, inform the user.
        byte error = light.getError();
        lux_printError(error);
    }

    return lux;
}


void imu_print(void)
{
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2
    imu::Vector<3> euler = bno_sensor.getVector(Adafruit_BNO055::VECTOR_EULER);

    /* Display the floating point data */
    printf("X: %f, Y: %f, Z: %f     |   ", euler.x(),euler.y(), euler.z());


    // Quaternion data
    imu::Quaternion quat = bno_sensor.getQuat();
    printf("qW: %f ", (float)quat.w());
    printf("qX: %f ", (float)quat.x());
    printf("qY: %f ", (float)quat.y());
    printf("qZ: %f  |   ", (float)quat.z());

    /* Display calibration status for each sensor. */
    uint8_t system, gyro, accel, mag = 0;
    bno_sensor.getCalibration(&system, &gyro, &accel, &mag);
    printf("CALIBRATION: Sys= %d, Gyro= %d, Accel= %d, Mag= %d \n",system,gyro,accel,mag);

    delay(BNO055_SAMPLERATE_DELAY_MS);
}

double* imu_getOrientation()
{
    imu::Vector<3> euler = bno_sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
    
    return &euler[0];
}


/* Return the gauge pressure in Pascal */
int getPressure_pa(adcPin sensorPin)
{
    int mvolt;

    mvolt = map(analogRead(sensorPin),0,1023,0,5000);

    return map(mvolt, 500, 4500, 0, 100*PSI_TO_PASCAL);
}

/* Return the gauge pressure in mH20 */
int getPressure_mH2O(adcPin sensorPin)
{
    int mvolt;

    mvolt = map(analogRead(sensorPin),0,1023,0,5000);

    return map(mvolt, 500, 4500, 0, 100*PSI_TO_PASCAL*PASCAL_TO_MH2O);
}


/* Return the distance measured by an IR sensor(2Y0A710) */
int get_IRdistance_cm(adcPin sensorPin)
{
    int distance;

    distance = map(analogRead(sensorPin),0,1023, 150, 0);

    return distance;
}
/**************************************************************************************************
*   sensor.cpp
**************************************************************************************************/