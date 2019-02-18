/*
 * @brief hobot version log
 * @author dc_neo(yan01.zhang)
 * @date 13/July/2017
 */

#ifndef HOBOT_LOG_INSTACE_H
#define HOBOT_LOG_INSTACE_H
#include <iostream>
#include "log4cpp/Category.hh"

namespace hobotlog {

class LogModule {
public:
    /*
     * @brief set configfile
     * @author dc_neo(yan01.zhang)
     * @date 13/July/2017
     */
    LogModule(const std::string& filename); 
    ~LogModule();
    // record debug message
    void debug(const std::string&);
    // record info message
    void info(const std::string&);
    // record warn message
    void warn(const std::string&);
    // record error message
    void error(const std::string&);
private:
    log4cpp::Category& _debugcate;
    log4cpp::Category& _infocate;
    log4cpp::Category& _warncate;
    log4cpp::Category& _errorcate;
};

}
#endif  // HOBOT_LOG_INSTACE_H
