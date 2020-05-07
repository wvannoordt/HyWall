#include "HyWall.h"
#include "ScreenOutput.h"
#include "Parallel.h"
#include <string>
#include <iostream>

namespace HyWall
{
    void WriteLine(int messageVerboseLevel, std::string message)
    {
        if (Parallel::pId == 0)
        {
            if (messageVerboseLevel <= settings.verboseLevel)
            {
                if (messageVerboseLevel==1)
                {
                    std::cout << "[I] HyWall :: " << message << std::endl;
                }
                else
                {
                    std::cout << "[Debug " << std::to_string(messageVerboseLevel) << "] HyWall :: " << message << std::endl;
                }
            }
        }
    }
}
