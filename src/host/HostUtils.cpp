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

    void BufferFill(double* array, int num, double bValue)
    {
        for (int i = 0;  i < num; i++) array[i] = bValue;
    }
    
    std::string bufStr(int num, int bufSize)
    {
        std::string output = std::to_string(num);
        while (output.length()<bufSize) output = " " + output;
        return output;
    }
}
