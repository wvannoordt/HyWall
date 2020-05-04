#include "HyWall.h"
#include "ScreenOutput.h"
#include "Parallel.h"
#include <string>

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
                    std::cout << "[I] " << message << std::endl;
                }
                else
                {
                    std::cout << "[Debug " << std::to_string(messageVerboseLevel) << "]: " << message << std::endl;
                }
            }
        }
    }
}
