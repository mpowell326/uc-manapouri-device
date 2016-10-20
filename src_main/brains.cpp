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
#include "thrusters.h"

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

/*          x,      y,      yaw,    pitch */
int* KP = [ 1,      1,      1,      1     ];
int* KI = [ 1,      1,      1,      1     ];
int* KD = [ 1,      1,      1,      1     ];

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


std::vector <int> Device::get_orientationControl()
{
    std::vector<int> percentages(4);
    percentages.assign ( 4, 0);

    /* YAW:  pid control to keep deice straight using imu data */
    yawController.calculate( COMPASS_BEARING, pose.yaw(), upTime - prevTime);


    /* PITCH:  pid control to keep deice level using imu data */
    pitchController.calculate( 0, pose.pitch(), upTime - prevTime)



    return percentages;

}


std::vector <int> Device::get_positionControl()
{
    std::vector<int> percentages(4);
    percentages.assign ( 4, 0 );

    /* VERTICAL: pid control using pressure data    */
    yController.calculate( invertElev+5 , pressure, upTime - prevTime );
    
    /* HORIZONTAL: use IR as virtual bumper */
    // xController.calculate( 0, position.x, upTime - prevTime );
    for (int i=0; i<4; i++)
    {
        // if ()
    }
    // possibly use acceleration and dt to get an idea of distance moved from centre

    // re-adjust estimated invertElev using IR data

    return percentages;

}


void Device::adjustPose()
{
    std::vector<int> motor_percentagesOri, motor_percentagesPos;

    motor_percentagesOri = get_orientationControl();
    motor_percentagesPos = get_positionControl();

    for( int i=0; i<4; i++)
    {
        motor_percentage[i] = motor_percentagesOri[i] * motor_percentagesPos[i];
    }
    
    // std::transform(motor_percentages1.begin( ), motor_percentages1.end( ), motor_percentages2.begin( ), motor_percentages1.begin( ),std::plus<int>( ));
    // motor_percentage_ = &get_orientationControl()[0];   //IMU
    // motor_percentage = motor_percentage + &get_positionControl()[0];     //IR and pressure
    
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
    d = rem >= 5 ? (pose.distance + 10 - rem ) : (pose.distance - rem);
    

    invertElev = tunnel_elevation[d-1110][1];
}


bool Device::test_services()
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

    // lux_init();
    // imu_init();

    // delay(3000);

    // Set up PID controllers
    xController.init(100, -100, KP(0), KI(0), KD(0) );
    yController.init(100, -100, KP(1), KI(1), KD(1) );
    yawController.init(100, -100, KP(2), KI(2), KD(2) );
    pitchController.init(100, -100, KP(3), KI(3), KD(3) );

    return true;
}


/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/

// CONSTRUCTOR 
Device::Device()
{
    cout << "Device created" << endl;
}


void Device::begin()
{
    cout << "Device is being initalized" << endl;
    operation_state = Boot;
    deployTime = 0;
    pose.distance = 1100;    //meters
    invertElev = 0;
}


void Device::begin(device_state init_state, int prev_uptime)
{
    cout << "Device is being re-initalized" << endl;
    operation_state = init_state;
    upTime = prev_uptime;
    deployTime = DEPLOY_TIME/2;             // this wiill need fixing!
    pose.distance = 1100;
    invertElev = 0;
}


void Device::readSensors()
{
    pressure = getPressure_mH2O(PRESSURE_SENSOR_PIN);

    IRtop = get_IRdistance_cm(IRup_PIN);
    IRleft = get_IRdistance_cm(IRleft_PIN);
    IRbottom = get_IRdistance_cm(IRdown_PIN);
    IRright = get_IRdistance_cm(IRright_PIN);

    // pose.orientation = imu_getOrientation();
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
            else
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
            if ( (lux > DARK_LUX)  || (upTime > RETRIEVE_TIME) ){
                operation_state = Retrieval;
            }
            break;

        case Retrieval:
            surfaceDevice();

            // flash_lights(RETRIEVAL)
            break;
        default:
            break;
    }

}


void Device::updateTravelTime(int time)
{
    prevTime = upTime;
    upTime = time;

    if( operation_state == Traverse)
    {
        // Estimated distance through tunnel
        pose.distance = ESTIMATED_VEL * ( upTime - deployTime) + 1100;   // Check units!!!!


        updateInvertElev();
    }
}

    
int Device::getInvertElev()
{
    return invertElev;
}

int Device::get_distance()
{
    return pose.distance;
}
/**************************************************************************************************
*   brains.cpp
**************************************************************************************************/