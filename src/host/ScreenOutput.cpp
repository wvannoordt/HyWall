#include "HyWall.h"
#include "ScreenOutput.h"
#include <string>

namespace HyWall
{
    void WriteLine(int messageVerboseLevel, std::string message)
    {
        if (messageVerboseLevel >= settings.verboseLevel)
        {
            std::cout << "[I] " << message << std::endl;
        }
    }
}
