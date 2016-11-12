/*!
*   \logger.h
*
*   Purpose:            Module to log various given data to file
*
*   Created:            10/11/2016  Morgan Powell
*   Last Modified:      
*/

#ifndef LOGGER_H
#define LOGGER_H


#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
using namespace std;
/*-----------------------------------------------------------------------------------------------*/
/*  Defines                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#define LOGGER Logger::GetLogger()

/*-----------------------------------------------------------------------------------------------*/
/*  Typedefs                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
class Logger
{
public:
    /**
    *   Logs a message
    *   @param sMessage message to be logged.
    */
    void Log(const std::string& sMessage);
    /**
    *   Variable Length Logger function
    *   @param format string for the message to be logged.
    */
    void Log(const char * format, ...);
    /**
    *   << overloaded function to Logs a message
    *   @param sMessage message to be logged.
    */
    Logger& operator<<(const string& sMessage);
    /**
    *   Funtion to create the instance of logger class.
    *   @return singleton object of logger class..
    */
    static Logger* GetLogger();



private:

    Logger();
    Logger(const Logger&){};             // copy constructor is private
    Logger& operator=(const Logger&){ return *this; };  // assignment operator is private
    ~Log();

    static const std::string _sFileName;
    static Logger* _pThis;
    static ofstream _Logfile;
};

/*-----------------------------------------------------------------------------------------------*/
/*  Function Prototypes                                                                          */
/*-----------------------------------------------------------------------------------------------*/
const std::string CurrentDateTime();




#endif /* LOGGER_H */

/**************************************************************************************************
*   End of header file logger.h
**************************************************************************************************/
