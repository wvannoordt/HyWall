#ifndef HOST_UTIL_H
#define HOST_UTIL_H

#define hy_max(a,b) (((a)>(b))?(a):(b))
#define hy_min(a,b) (((a)<(b))?(a):(b))
#define hy_abs(a) (((a)<(0))?(-(a)):(a))
#include <string>
#include <unistd.h>

namespace HyWall
{
    std::string to_estring(double num);
    void BufferFill(double* array, int num, double bValue);
    std::string bufStr(int num, int bufSize);
}

#endif
