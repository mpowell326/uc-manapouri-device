/*!
*   \brains.cpp
*
*   Purpose:            Control the device reacting to events from sensors. 
*                       Boot -> Deployment
*                       Deployment -> Traversing
*                       Traversing -> Retrieval
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include "brains.h"
#include "bbb_config.h"
#include "Wiring.h"
#include "sensors.h"
#include "tunnel_elevation.h"


/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/

#define COMPASS_BEARING 304     // Roughly, needs to be checked and adujsted for magnetic

#define DEPLOY_TIME     5*60    // seconds
#define RETRIEVE_TIME   90*60   // seconds

#define DARK_LUX        100     // lux to consider having reached tunnel exit

#define ESTIMATED_VEL   3.5     // m/s
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

void Device::surfaceDevice()
{
    // Check if device is in the water (below 1mH20)
    if(pressure >= 1)
    {
        // setTarget();
        adjustPose();
    }
    else
    {
        // Disable Thrusters
        motor_setSpeed( MOTOR1_PIN, 0 );
        motor_setSpeed( MOTOR2_PIN, 0 );
        motor_setSpeed( MOTOR3_PIN, 0 );
        motor_setSpeed( MOTOR4_PIN, 0 );
    }
}

void Device::get_orientationControl()
{
    // pid control to keep deice straight using imu data

}


void Device::get_positionControl()
{
    // pid control using pressure data

    // possibly use acceleration and time to get an idea of distance moved

    // readjust using IR data

}


void Device::adjustPose()
{

    motor_percentage = get_orientationControl();   //IMU
    motor_percentage += get_positionControl();     //IR and pressure

    /* Adjust the motors to the new speed */
    motor_setSpeed(MOTOR1_PIN, motor_percentage[0]);
    motor_setSpeed(MOTOR2_PIN, motor_percentage[1]);
    motor_setSpeed(MOTOR3_PIN, motor_percentage[2]);
    motor_setSpeed(MOTOR4_PIN, motor_percentage[3]);
}


void Device::updateInvertElev()
{
    int d;


    int rem = pose.distance % 10;

    // Round either up or down to nearest 10 m
    d = rem >= 5 ? (pose.distance + 10 - rem ) : (pose.distance - rem)
       
    
    invertElev = tunnel_elevation{[d][1]};
}


void Device::test_services()
{
    // test stuff to see if it has initialised and is running correctly
    return true;
}


bool Device::init()
{
    printf("Nemo is waking up...\n");
    motor_init(MOTOR1_PIN);
    motor_init(MOTOR2_PIN);
    motor_init(MOTOR3_PIN);
    motor_init(MOTOR4_PIN);

    lux_init();
    imu_init();
    
    delay(3000);
    return true
}


/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/

// CONSTRUCTOR 
Device::Device()
{
    cout << "Device created" << endl;
}


Device::begin()
{
    cout << "Device is being initalized" << endl;
    operation_state = Boot;
    deployTime = 0;
    pose.distance = 1100;    //meters
}


Device::begin(device_state init_state, int prev_time)
{
    cout << "Device is being re-initalized" << endl;
    operation_state = init_state;
    upTime = prev_time;
    deployTime = DEPLOY_TIME/2;             // this wiill need fixing!
    pose.distance = 1100;
}


void Device::readSensors()
{
    pressure = getPressure_mH2O(adcPin sensorPin);

    IRtop = get_IRdistance_cm(adcPin sensorPin);
    IRleft = get_IRdistance_cm(adcPin sensorPin);
    IRbottom = get_IRdistance_cm(adcPin sensorPin);
    IRright = get_IRdistance_cm(adcPin sensorPin);

    orientation = bno_sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
}


void Device::state_controller()
{
    switch(operation_state)
    {
        case Boot:
            bool success;
            success = init();
            success &= test_services();

            if (success){
                operation_state = Deployment;
            }
            else (!success)
            {
                // flash_lights(ERROR);
            }
            break;
    

        case Deployment:
            // Check if device is in the water (below 1mH20)
            if ( pressure > 1){
                operation_state = Traverse;
            }
            // Likely that the pressure sensor is not working
            if ( upTime >= DEPLOY_TIME){
                deployTime = upTime;
                operation_state = Traverse;
            }
            break;

        case Traverse:
            // Reposition device in centre and keep straight
            adjustPose();

            // Device has reached end of tunnel
            if ( (lux > DARK_LUX)  or (upTime => RETRIEVE_TIME) ){
                operation_state = Retrieval;
            }
            break;

        case Retrieval:
            surfaceDevice();

            // flash_lights(RETRIEVAL)
            break;
    }

}


void updateTravelTime(int time)
{
    upTime = time;

    if( state > Deployment)
    {
        // Estimated distance through tunnel
        pose.distance += ESTIMATED_VEL * ( upTime - deployTime) ;   // Check units!!!!
        updateInvertElev();
    }

}

    

/**************************************************************************************************
*   brains.cpp
**************************************************************************************************/