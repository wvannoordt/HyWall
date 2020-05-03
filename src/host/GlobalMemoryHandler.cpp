#include "BufferFlags.h"
#include "GlobalMemoryHandler.h"
#include <string>
namespace HyWall
{
    GlobalMemoryHandler::GlobalMemoryHandler(void){}

    void GlobalMemoryHandler::SetSize(int numPoints_in, int rayDim_in)
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

    void GlobalMemoryHandler::SetUserAssociatedVariable(std::string name, double* ptr)
    {

    }

    template <typename vartype>
    void GlobalMemoryHandler::AddStaticVariable(std::string name, int num_per_ray, int dimension, const int managemode)
    {

    }
}
