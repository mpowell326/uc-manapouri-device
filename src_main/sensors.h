/*!
*   \sensors.h
*
*   Purpose:            Module to read various sensor values
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

#ifndef SENSOR_H
#define SENSOR_H


// using namespace std;
#include <SparkFunTSL2561.h>
#include "Wiring.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
// #include <vector>
/*-----------------------------------------------------------------------------------------------*/
/*  Defines                                                                                      */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/*  Typedefs                                                                                     */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/*  Function Prototypes                                                                          */
/*-----------------------------------------------------------------------------------------------*/
void lux_init();
void imu_init();
double getLux();
void imu_print(void);
double* imu_getOrientation();
int getPressure_pa(adcPin sensorPin);
int getPressure_mH2O(adcPin sensorPin);
int get_IRdistance_cm(adcPin sensorPin);



#endif /* SENSOR_H */

/**************************************************************************************************
*   End of header file sensors.h
**************************************************************************************************/