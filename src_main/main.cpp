/*!
*   main.cpp
*
*   Purpose:            Program to run on beaglebone black for controlling the tailrace inspcetion device (Nemo)
*
*   Created:            xx/yy/2016  ...
*   Last Modified:      
*/

#include <stdio.h>
#include <ctime>
#include "bbb_config.h"
#include "brains.h"


#define LOG_FILENAME "../device_log.txt"




void logData(* fptr, *char data)
{
    printf(data);
}



int main(void)
{
    // System Time
    std::clock_t tics = std::clock();
    std::clock_t time = tics / (double) CLOCKS_PER_SEC;

    // Create deive object
    Device nemo;

    // Open file to write log to
    logFile = openFile( LOG_FILENAME )
    

    // Check if reboot has occured. If so continue from previous data...
    if(getPrevLog(logFile) != None)
    {
        nemo.begin(prevState, prevTime);
    }
    else{
        nemo.begin();
        logFile_init();
    }
    

    while(1)
    {
        // Update the time
        tics = std::clock();
        time = tics / (double) CLOCKS_PER_SEC;
        nemo.updateTravelTime( time );

        // Get the current readings from the sensors
        nemo.readSensors();

        // Run appropriate functions/operations for current state
        nemo.state_controller();

        // Log recorded data to disk
        logData(nemo.get_logData());


        delay(1000);         // Probably should add a basic task scheduler
    }
    return 0;
}
