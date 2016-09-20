/*!
*   \motors.cpp
*
*   Purpose:            Provide an interface to control the T100 thrusters with the use of PWMs
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include "motors.h"
#include "bbb_config.h"
#include "Wiring.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define PWM_FREQ 50
#define PWM_PERIOD 1000000/PWM_FREQ
#define PWM_STOP_SIGNAL 1500

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

// int motor_getAgjustment()
// {
//     // return value for motor adujsted for which direction
// }


  

/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/
void motot_init(Pin motorpin)
{
    pinMode(P8_10, OUTPUT);
    setTimePeriod (motorpin, PWM_PERIOD);
    setPulseWidth (motorpin, PWM_STOP_SIGNAL);
}

void motor_setSpeed(Pin motorpin, int percentage)
{
    int pwm_width;
    // Calculate signal value, which should be between 1100us(max reverse) and 1900us(max forwards)
    map(percentage, -100, 100, 1100,1900); 
    
    // Add calibration/adjustment for motor differing in power for forwards/backwards
    // motor_getAgjustment();

    // Set the PWM at the new speed
    setPulseWidth (motorpin, pwm_width);

}

/**************************************************************************************************
*   adc.c
**************************************************************************************************/