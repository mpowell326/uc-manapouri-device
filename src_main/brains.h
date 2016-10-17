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
    // Device pose
    imu::Vector<3> orientation;
    imu::Vector<2> position;
    int distance;
}


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
    int IR1, IR2, IR3, IR4;

    // Current Motor speed/percentage
    int[4] motor_percentage;


    void surfaceDevice();
    void get_orientationControl();
    void get_positionControl();
    void adjustPose();
    void updateInvertElev();
    bool test_services();
    bool init();

public:
    void Device(void);
    void begin();
    void begin(device_state init_state, int prev_time);
    void readSensors();
    void state_controller();
    void updateTravelTime(int time);
}




#endif /* BRAINS_H */

/**************************************************************************************************
*   End of header file brains.h
**************************************************************************************************/