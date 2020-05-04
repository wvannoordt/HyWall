#include "BufferFlags.h"
#include "GlobalMemoryHandler.h"
#include "ScreenOutput.h"
#include <string>
namespace HyWall
{
    GlobalMemoryHandler::GlobalMemoryHandler(void){}

    void GlobalMemoryHandler::SetSize(int numPoints_in, int rayDim_in)
    {
        rayDim = rayDim_in;
        localTotalPoints = numPoints_in;
        localCpuPoints = localTotalPoints;
        localGpuPoints = 0;
        globalGpuPoints = 0;
        globalCpuPoints = 0;
        globalTotalPoints = 0;
        numGlobalVariables = 0;
    }

    void GlobalMemoryHandler::SetUserAssociatedVariable(std::string name, double* ptr)
    {
        WriteLine(2, "Setting user-associated variable \"" + name + "\"");
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                isAllocated[i] = true;

            }
        }
    }

    template void GlobalMemoryHandler::AddStaticVariable<double>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<float>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<int>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<bool>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<char>(std::string, int, int, const int);
    template <typename vartype> void GlobalMemoryHandler::AddStaticVariable(std::string name, int numPerRay, int dimension, const int managemode)
    {
        //IMPORTANT: will need to add buffer mirroring for gpu variables
        size_t newVariableSize = numPerRay * dimension * localCpuPoints * sizeof(vartype);

        userHasProvided[numGlobalVariables] = false;
        isAllocated[numGlobalVariables]     = false;
        variableNames[numGlobalVariables]   = name;
        manageModes[numGlobalVariables]     = managemode;
        bufferSizes[numGlobalVariables]     = newVariableSize;

        WriteLine(3, "Declaring static associated variable:");
        WriteLine(3, "name:       " + name);
        WriteLine(3, "managemode: " + std::to_string(managemode));
        WriteLine(3, "numPerRay:  " + std::to_string(numPerRay));
        WriteLine(3, "dimension:  " + std::to_string(dimension));
        WriteLine(3, "size:       " + std::to_string(newVariableSize));

        numGlobalVariables++;
    }
}
