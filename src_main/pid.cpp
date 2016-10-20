/*!
*   \pid.cpp
*
*   Purpose:            A simple PID class
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/


/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include <iostream>
#include <cmath>
#include "pid.h"

using namespace std;

/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/
void PID::init( double max, double min, double Kp, double Kd, double Ki )
{
    _max = max;
    _min = min;
    _Kp = Kp;
    _Kd = Kd;
    _Ki = Ki;
    _pre_error = 0;
    _integral = 0;

}

double PID::calculate( double setpoint, double pv, double dt )
{
    
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > _max )
        output = _max;
    else if( output < _min )
        output = _min;

    // Save error to previous error
    _pre_error = error;

return output;
}

PID::~PID()
{
}

/**************************************************************************************************
*   pid.cpp
**************************************************************************************************/