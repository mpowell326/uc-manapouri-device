/*!
*   \brains.h
*
*   Purpose:            Control the device reacting to events from sensors. 
*                       Boot -> Deployment
*                       Deployment -> Traversing
*                       Traversing -> Retrieval
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

#ifndef BRAINS_H
#define BRAINS_H


#include <vector>
#include "pid.h"

using namespace std;

/*-----------------------------------------------------------------------------------------------*/
/*  Defines                                                                                      */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/*  Typedefs                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
// Device operation states
typedef enum {
    Boot,
    Deployment,
    Traverse,
    Retrieval
} device_state;


class Pose {
public:
    // Device pose
    double * orientation;
    double position[2];
    int distance;

    double yaw() const { return orientation[0]; }
    double pitch() const { return orientation[1]; }
    double roll() const { return orientation[2]; }


    double x() const { return position[0]; }
    double y() const { return position[1]; }
};


class Device {
    // Opertion state
    device_state operation_state;

    // Time since boot/Deployment
    double upTime;
    double prevTime;
    double deployTime;

    // Estimated current invert elevation of tunnel
    int invertElev;

    // Current Pose of device
    Pose pose;

    // Current sensor readings
    int pressure;
    int IRtop, IRleft, IRbottom, IRright;
    double lux;

    // Current Motor speed/percentage
    int motor_percentage[4];

    // PID controllers
    PID xController;
    PID yController;
    PID yawController;
    PID pitchController;
    double yTimer;
    double xTimer;


    void surfaceDevice();
    std::vector<int> get_orientationControl();
    std::vector<int> get_positionControl();
    void adjustPose();
    void updateInvertElev();
    bool test_services();
    bool init();

public:
    Device(void);
    void begin();
    void begin(device_state init_state, int prev_uptime);
    void readSensors();
    void state_controller();
    void updateTravelTime(double time);
    int getInvertElev();
    int get_distance();
};




#endif /* BRAINS_H */

/**************************************************************************************************
*   End of header file brains.h
**************************************************************************************************/