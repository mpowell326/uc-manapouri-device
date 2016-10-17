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

using namespace std;
#include <vector>

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
    double position[3];
    int distance;
};


class Device {
    // Opertion state
    device_state operation_state;

    // Time since boot/Deployment
    int upTime;
    int prevTime;
    int deployTime;

    // Estimated current invert elevation of tunnel
    int invertElev;

    // Current Pose of device
    Pose pose;

    // Current sensor readings
    int pressure;
    int IRtop, IRleft, IRbottom, IRright;
    double lux;

    // Current Motor speed/percentage
    int * motor_percentage;


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
    void begin(device_state init_state, int prev_time);
    void readSensors();
    void state_controller();
    void updateTravelTime(int time);
    int getInvertElev();
    int get_distance();
};




#endif /* BRAINS_H */

/**************************************************************************************************
*   End of header file brains.h
**************************************************************************************************/