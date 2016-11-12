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
#include "pid.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/

#define COMPASS_BEARING 304     // Roughly, needs to be checked and adujsted for magnetic

#define DEPLOY_TIME     30*60   // seconds
#define RETRIEVE_TIME   90*60   // seconds

#define DARK_LUX        3000     // lux to consider having reached tunnel exit

#define ESTIMATED_VEL   3.5     // m/s

#define DISTANCE_2_WALL 125     
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
int KP[4] = { 1,      1,      1,      1     };
int KI[4] = { 0,      0,      0,      0     };
int KD[4] = { 0,      0,      0,      0     };

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
        // adjustPose();
        motor_stopALL();
    }
    else
    {
        // Disable Thrusters
        motor_stopALL();
    }
}


std::vector <int> Device::get_orientationControl()
{
    std::vector<int> percentages(4);
    percentages.assign ( 4, 0);
    double temp_var;

    /* YAW:  pid control to keep device straight using imu data */
    temp_var = yawController.calculate( bearing_target, pose.yaw(), upTime - prevTime);   //maybe have setpoint as 0 and pv as + bearing to give more resolution
    percentages[0] = -temp_var;
    percentages[3] = temp_var;

    /* PITCH:  pid control to keep device level using imu data */
    temp_var = pitchController.calculate( 0, pose.pitch(), upTime - prevTime);
    percentages[1] = -temp_var;
    percentages[2] = temp_var;



    return percentages;

}


std::vector <int> Device::get_positionControl()
{
    std::vector<int> percentages(4);
    percentages.assign ( 4, 0 );
    int pushOffset_y, pushOffset_x;
    int temp_var;

    /* VERTICAL: pid control using pressure data    */                                                          // perhaps add kalman filter for this
    // temp_var = yController.calculate( invertElev+5 , pressure, upTime - prevTime );
    // percentages[2] = temp_var;
    // percentages[3] = temp_var;


    /* VERTICAL: virtual bumper using IR    */
    if (IRtop < DISTANCE_2_WALL)
    {
        // re-adjust/calibrate estimated invertElev using IR data
        yTimer = 5;
        pushOffset_y = -80; //down
    }
    if (IRbottom < DISTANCE_2_WALL)
    {
        // re-adjust/calibrate estimated invertElev using IR data
        yTimer = 5;
        pushOffset_y = 40; //up
    }

    /* Decrement verticl timer and add push if needed still */
    yTimer -= (upTime - prevTime);
    if ( yTimer > 0.5 )
    {
        percentages[1] += pushOffset_y;
        percentages[2] += pushOffset_y;
    }


    /* HORIZONTAL: use IR as virtual bumper */
    // xController.calculate( 0, pose.x(), upTime - prevTime );
    if (IRleft < DISTANCE_2_WALL)
    {
        xTimer = 5;
        pushOffset_x = 70; //right
    }
    if (IRright < DISTANCE_2_WALL)
    {
        pushOffset_x = -70; //left
    }

    /* Decrement horozontal timer and add push if needed still */
    xTimer -= (upTime - prevTime);
    if ( xTimer > 0.5 )
    {
        percentages[0] = pushOffset_x;
        percentages[3] = pushOffset_x;
    }



    /* 
    possibly use acceleration and dt to get an idea of distance moved from centre
    i.e  pose.x() += get_acc_x()*dt;
         pose.y() += get_acc_y()*dt;
    would then need to fuse pose.y and pressure data together.
    */


    return percentages;

}


void Device::adjustPose()
{
    std::vector<int> motor_percentagesOri, motor_percentagesPos;

    motor_percentagesOri = get_orientationControl();
    motor_percentagesPos = get_positionControl();

    for( int i=0; i<4; i++)
    {
        motor_percentage[i] = motor_percentagesOri[i] ;//+ motor_percentagesPos[i];
        printf("%d\n", motor_percentage[i]);
    }
    // std::transform(motor_percentages1.begin( ), motor_percentages1.end( ), motor_percentages2.begin( ), motor_percentages1.begin( ),std::plus<int>( ));
    // motor_percentage_ = &get_orientationControl()[0];   //IMU
    // motor_percentage = motor_percentage + &get_positionControl()[0];     //IR and pressure
    
    /* Adjust the motors to the new speed */
    // motor_setSpeed(MOTOR_F_H, motor_percentage[0]);
    // motor_setSpeed(MOTOR_F_V, motor_percentage[1]);
    // motor_setSpeed(MOTOR_R_V, motor_percentage[2]);
    // motor_setSpeed(MOTOR_R_H, motor_percentage[3]);
}


void Device::updateInvertElev()
{
    int d;


    int rem = pose.distance % 10;

    // Round either up or down to nearest 10 m
    d = rem >= 5 ? (pose.distance + 10 - rem ) : (pose.distance - rem);
    // printf("i = %d, r = %d, distance=%d \n",(d-1110)/10, rem, pose.distance);

    invertElev = tunnel_elevation[(d-1110)/10][1];                                      // Possiby could interpolate to get a more accurate reading
}


bool Device::test_services()
{
    // test stuff to see if it has initialised and is running correctly
    
    printf("Testing MOTOR_F_H\n");
    motor_setSpeed( MOTOR_F_H, 10);
    delay(1000);
    motor_setSpeed( MOTOR_F_H, 0 );

    printf("Testing MOTOR_F_V\n");
    motor_setSpeed( MOTOR_F_V, 10);
    delay(1000);
    motor_setSpeed( MOTOR_F_V, 0 );

    printf("Testing MOTOR_R_V\n");
    motor_setSpeed( MOTOR_R_V, 10);
    delay(1000);
    motor_setSpeed( MOTOR_R_V, 0 );

    printf("Testing MOTOR_R_H\n");
    motor_setSpeed( MOTOR_R_H, 10);
    delay(1000);
    motor_setSpeed( MOTOR_R_H, 0 );

    return true;
}


bool Device::init()
{
    printf("Nemo is waking up...\n");
    motor_init(MOTOR_F_H);
    motor_init(MOTOR_F_V);
    motor_init(MOTOR_R_V);
    motor_init(MOTOR_R_H);

    delay(3000);

    lux_init();
    imu_init();

    delay(3000);

    // Set up PID controllers
    xController.init(100, -100, KP[0], KD[0], KI[0] );
    yController.init(100, -100, KP[1], KD[1], KI[1] );
    yawController.init(100, -100, KP[2], KD[2], KI[2] );
    pitchController.init(100, -100, KP[3], KD[3], KI[3] );

    double * temp_var = imu_getOrientation();
    pose.orientation[0] = temp_var[0];
    pose.orientation[1] = temp_var[1];
    pose.orientation[2] = temp_var[2];
    bearing_target = pose.orientation[0];

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
    deployTime = DEPLOY_TIME/2;                                 // @@@ this will need fixing!
    pose.distance = 1100;
    invertElev = 0;
}


void Device::readSensors()
{
    // pressure = getPressure_mH2O(PRESSURE_SENSOR_PIN);
    pressure = 2;
    IRtop = get_IRdistance_cm(IRup_PIN);
    IRleft = get_IRdistance_cm(IRleft_PIN);
    IRbottom = get_IRdistance_cm(IRdown_PIN);
    IRright = get_IRdistance_cm(IRright_PIN);
    lux = getLux();

    double * temp_var = imu_getOrientation();
    pose.orientation[0] = temp_var[0];
    pose.orientation[1] = temp_var[1];
    pose.orientation[2] = temp_var[2];

    // imu_print();
    printf("                        Target Yaw: %f\n", bearing_target);
    printf("                        Yaw: %f, Pitch: %f, Lux: %f\n", pose.yaw(), pose.pitch(), lux);
    printf("                        IRtop: %d, IRleft: %d, IRbottom: %d, IRright: %d\n", IRtop, IRleft, IRbottom, IRright);
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
            printf("                    Pressure: %d\n", pressure);
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
            // adjustPose();

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


void Device::updateTravelTime(double time)
{
    prevTime = upTime;
    upTime = time;
    printf("dt: %f\n", upTime-prevTime);
    if( operation_state == Traverse)
    {
        // Estimated distance through tunnel
        pose.distance = ESTIMATED_VEL * (int)( upTime - deployTime) + 1100;   // Check units!!!!


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