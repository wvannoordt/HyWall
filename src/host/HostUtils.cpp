#include <string>
#include "HostUtils.h"
#include <cstring>
namespace HyWall
{
    std::string to_estring(double num)
    {
        char buffer[32];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%g", num);
        std::string strObj4(buffer);
        return strObj4;
    }
}
