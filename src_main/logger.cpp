/*!
*   \logger.cpp
*
*   Purpose:            Module to log various given data to file
*
*   Created:            10/11/2016  Morgan Powell
*   Last Modified:      
*/

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include "logger.h" 
#include <iostream>
#include <string>
#include <time.h>

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define FILEPATH "/media/card"


/*-----------------------------------------------------------------------------------------------*/
/* Variables                                                                                     */
/*-----------------------------------------------------------------------------------------------*/
Logger* Logger::_pThis = NULL;
ofstream Logger::_Logfile;


// const string Logger::m_sFileName = "Log.txt";









/*-----------------------------------------------------------------------------------------------*/
/* Private Functions:                                                                            */
/*-----------------------------------------------------------------------------------------------*/
Logger::Logger()
{

}
  
Log::~Log(){
  _Logfile.close();
  _pThis = NULL;
}
/*-----------------------------------------------------------------------------------------------*/
/* Public Functions                                                                              */
/*-----------------------------------------------------------------------------------------------*/
Logger* Logger::GetLogger(){
    if (_pThis == NULL)
    {
        _pThis = new Logger();
    }
    return _pThis;
}
 

Logger::Init(char* filename)
{

    _Logfile.open(FILEPATH + filename, ios::out | ios::app);

}


void Logger::Log(const string& sMessage)
{
    _Logfile <<  Util::CurrentDateTime() << ":\t";
    _Logfile << sMessage << "\n";
}


void Logger::Log(const char * format, ...)
{
    char* sMessage = NULL;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    vsprintf_s(sMessage, nLength, format, args);
    //vsprintf(sMessage, format, args);
    _Logfile << Util::CurrentDateTime() << ":\t";
    _Logfile << sMessage << "\n";
    va_end(args);
 
    delete [] sMessage;
}
 

Logger& Logger::operator<<(const string& sMessage)
{
    _Logfile << "\n" << Util::CurrentDateTime() << ":\t";
    _Logfile << sMessage << "\n";
    return *this;
}



// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string CurrentDateTime()
{
    time_t     now = time(NULL);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}



/**************************************************************************************************
*   logger.cpp
**************************************************************************************************/