#ifndef LOGGER_H
#define	LOGGER_H

#include <string>
#include <iostream>

/** Logger class */
class Logger
{
    private:
        static int level;
        
        Logger() {};
        Logger(Logger const&);
        void operator=(Logger const&);
        
    public:
        /** Get logger singleton */
        static Logger& getInstance()
        {
            static Logger instance;
            return instance;
        }
        
        /** Set error level */
        static void setLevel(const int _level)
        {
            level = _level;
        }
        
        /** Trace info */
        static void info(const std::string &str)
        {
            if(level > 0) {
                std::cout << str << std::endl;
            }
        }
        
        /** Trace error */
        static void error(const std::string &str)
        {
            std::cout << str << std::endl;
        }
};

#endif

