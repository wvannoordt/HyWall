#include "BufferFlags.h"
#include "GlobalMemoryHandler.h"
#include <string>
namespace HyWall
{
    GlobalMemoryHandler::GlobalMemoryHandler(void){}

    GlobalMemoryHandler::GlobalMemoryHandler(int numPoints_in, int rayDim_in)
    {
        rayDim = rayDim_in;
        localTotalPoints = numPoints_in;
        //int localCpuPoints;
        //int localGpuPoints;
        //int localTotalPoints;
        //int globalGpuPoints;
        //int globalCpuPoints;
        //int globalTotalPoints;
    }

    template <typename vartype>
    void GlobalMemoryHandler::AddStaticVariable(std::string name, int num_per_ray, int dimension, const int managemode)
    {

    }
}
