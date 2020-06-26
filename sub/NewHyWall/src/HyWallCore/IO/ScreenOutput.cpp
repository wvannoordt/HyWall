#include "HyWall.h"
#include "ScreenOutput.h"
#include "Parallel.h"
#include <string>
#include <iostream>
#include "Config.hx"

namespace HyWall
{
    void internal_output(int messageVerboseLevel, std::string message, const char* file, const int line)
    {
        internal_output(messageVerboseLevel, message, "[I] HyWall ::", file, line);
    }

    void internal_output(int messageVerboseLevel, std::string message, std::string styleString, const char* file, const int line)
    {
        if (Parallel::pId == 0)
        {
            //if (messageVerboseLevel <= settings.verboseLevel)
            if (true)
            {
                WRITE_FILE_LINE;
                std::cout << "TODO: settings/verboselevel" << std::endl;
                if (messageVerboseLevel==1)
                {
                    std::cout << styleString << " " << message << std::endl;

                }
                else
                {
                    std::cout << "[Debug " << std::to_string(messageVerboseLevel) << "] HyWall :: " << message << std::endl;
                }
            }
        }
    }
}
