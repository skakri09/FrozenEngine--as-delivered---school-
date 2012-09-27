#pragma once
#include <string>
#include <fstream>
enum Level{
    INFO,WARN,ERRORX,CRITICAL
};
/*
Main logsystem class. All calls will be direcetd through this singleton.
*/
class LogSys
{
public: 
    static LogSys& Instance();
    void Write(const std::string &source,Level level,const std::string &message);
private:  LogSys(const std::string &fileName); // ctor hidden  
           LogSys(LogSys const&); // copy ctor hidden  
           LogSys& operator=(LogSys const&); // assign op. hidden  
           ~LogSys(); // dtor hidden
           static const std::string SEPARATOR;
           bool toConsole;
           std::fstream fileOut;
};

;
static std::string levelNames[] = {
    "Info", "Warn", "Error", "Critical" 
};