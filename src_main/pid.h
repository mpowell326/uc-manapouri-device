/*!
*   pid.h
*
*   Purpose:            A simple PID class
*
*   Created:            xx/yy/2016  ...    
*/

#ifndef PID_H
#define PID_H



/*-----------------------------------------------------------------------------------------------*/
/*  Typedefs                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
class PID
{
    public:
        void init( double max, double min, double Kp, double Kd, double Ki );

        // Returns the manipulated variable given a setpoint and current process value
        double calculate( double setpoint, double pv, double dt );

        ~PID();

    private:
        double _dt;         // dt -  loop interval time
        double _max;        // max - maximum value of manipulated variable
        double _min;        // min - minimum value of manipulated variable
        double _Kp;         // Kp -  proportional gain
        double _Kd;         // Kd -  derivative gain
        double _Ki;         // Ki -  Integral gain
        double _pre_error;
        double _integral;
};



#endif /* PID_H */

/**************************************************************************************************
*   End of header file pid.h
**************************************************************************************************/