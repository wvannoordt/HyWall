#include "BufferFlags.h"
#include "GlobalMemoryHandler.h"
#include "ScreenOutput.h"
#include "DebugTools.h"
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
        initializePoliciesWereApplied = false;
    }

    void GlobalMemoryHandler::SetUserAssociatedVariable(std::string name, double* ptr)
    {
        WriteLine(2, "Setting user-associated variable \"" + name + "\"");
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                if (!HasFlag(manageModes[i], bflag::userCanProvide) && !HasFlag(manageModes[i], bflag::userMustProvide)) __erkill("User is not allowed to provide aux variable \"" + name + "\"");
                isAllocated[i] = true;
                userHasProvided[i] = true;
                hostBuffers[i] = ptr;
                WriteLine(3, "Found \"" + name + "\"");
                return;
            }
        }
        __erkill("Could not set appropriate global variable for \"" + name + "\". Variable was probably never declared.");
    }

    void* GlobalMemoryHandler::GetVariable(std::string name)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                return hostBuffers[i];
            }
        }
        __erkill("Could not get appropriate global variable for \"" + name + "\"");
    }

    template void GlobalMemoryHandler::AddStaticVariable<double>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<float>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<int>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<bool>(std::string, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<char>(std::string, int, int, const int);
    template <typename vartype> void GlobalMemoryHandler::AddStaticVariable(std::string name, int numPerRay, int dimension, const int manageMode)
    {
        //IMPORTANT: will need to add buffer mirroring for gpu variables
        size_t newVariableSize = numPerRay * dimension * localCpuPoints * sizeof(vartype);
        if (HasFlag(manageMode, bflag::serialHostUsage)) newVariableSize = numPerRay * dimension * sizeof(vartype);
        int newUpperAccessBound = newVariableSize / sizeof(vartype);

        userHasProvided[numGlobalVariables] = false;
        isAllocated[numGlobalVariables]     = false;
        variableNames[numGlobalVariables]   = name;
        manageModes[numGlobalVariables]     = manageMode;
        bufferSizes[numGlobalVariables]     = newVariableSize;
        accessBounds[numGlobalVariables]    = newUpperAccessBound;

        __dump(numGlobalVariables);

        WriteLine(3, "Declaring static associated variable:");
        WriteLine(3, "name:       " + name);
        WriteLine(3, "managemode: " + std::to_string(manageMode));
        WriteLine(3, "numPerRay:  " + std::to_string(numPerRay));
        WriteLine(3, "dimension:  " + std::to_string(dimension));
        WriteLine(3, "size:       " + std::to_string(newVariableSize));

        numGlobalVariables++;
    }

    void GlobalMemoryHandler::ApplyInitializationPolicies(void)
    {
        initializePoliciesWereApplied = true;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            int manageMode = manageModes[i];

            if (HasFlag(manageMode, bflag::userMustProvide) && !userHasProvided[i]) __erkill("Failed to find required user-provided variable \"" + variableNames[i] + "\"");

            bool allocateHostMirror = true;
            if (userHasProvided[i]) allocateHostMirror = false;
            if (isAllocated[i]) allocateHostMirror = false;

            bool allocateDeviceMirror = true;
            if (HasFlag(manageMode, bflag::auxilary)) allocateDeviceMirror = false;

            if (allocateHostMirror)
            {
                isAllocated[i] = true;
                WriteLine(2, "Allocating host-side buffer for \"" + variableNames[i] + "\", size " + std::to_string(bufferSizes[i]) + ".");
                WriteLine(4, "(" + std::to_string(bufferSizes[i]) + " = " + ((localCpuPoints>0)?std::to_string(bufferSizes[i]/localCpuPoints):"( ???? )") + "x" + std::to_string(localCpuPoints) + ")");
                hostBuffers[i] = malloc(bufferSizes[i]);
            }

        }
    }

    void GlobalMemoryHandler::ApplyRuntimePolicies(void)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {

        }
    }

    void GlobalMemoryHandler::ApplyFinalizationPolicies(void)
    {
        if (!initializePoliciesWereApplied) return;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            int manageMode = manageModes[i];

            bool freeHostMirror = true;
            if (userHasProvided[i]) freeHostMirror = false;
            if (!isAllocated[i]) freeHostMirror = false;

            bool freeDeviceMirror = true;

            if (freeHostMirror)
            {
                free(hostBuffers[i]);
                WriteLine(2, "Freeing host-side buffer for \"" + variableNames[i] + "\"");
            }
        }
    }
}
