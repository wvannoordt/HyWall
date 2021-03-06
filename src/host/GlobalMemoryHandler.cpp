#include "BufferFlags.h"
#include "GlobalMemoryHandler.h"
#include "ScreenOutput.h"
#include "DebugTools.h"
#include <string>
#include <vector>
#include "HyWall.h"
namespace HyWall
{
    GlobalMemoryHandler::GlobalMemoryHandler(void){initializePoliciesWereApplied=false;localTotalPoints=0;}

    void GlobalMemoryHandler::SetSize(int numPoints_in, int rayDim_in)
    {
        balancer = NULL;
        rayDim = rayDim_in;
        localTotalPoints = numPoints_in;
        localCpuPoints = localTotalPoints;
        localGpuPoints = 0;
        globalGpuPoints = 0;
        globalCpuPoints = 0;
        globalTotalPoints = 0;
        numGlobalVariables = 0;
        localCpuPointsNative = numPoints_in;
        localGpuPointsNative = 0;
        localTotalPointsNative = localTotalPoints;
        if (settings.loadBalance)
        {
            balancer = new LoadBalancer();
            balancer->Balance(localCpuPointsNative, &localCpuPoints);
        }
        initializePoliciesWereApplied = false;
        Parallel::Allreduce(&localTotalPoints, &globalTotalPoints, 1, HY_INT, HY_SUM);
        Parallel::Allreduce(&localGpuPoints,   &globalGpuPoints,   1, HY_INT, HY_SUM);
        Parallel::Allreduce(&globalCpuPoints,  &globalCpuPoints,   1, HY_INT, HY_SUM);
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
                if (userHasProvided[i]) __erkill("User is not allowed to set \"" + name + "\" twice");
                userHasProvided[i] = true;
                hostBuffers[i] = ptr;
                // loadBalanceBuffer[i] = NULL;
                // hasLoadBalanceBuffer = true;
                // loadBalanceBufferIsAllocated[i] = false;
                // if ()
                elementCount[i] = -999;
                WriteLine(3, "Found \"" + name + "\"");
                return;
            }
        }
        __erkill("Could not set appropriate global variable for \"" + name + "\". Variable was probably never declared.");
    }
    
    std::vector<std::string> GlobalMemoryHandler::GetVarsByStartingString(std::string startStr)
    {
        std::vector<std::string> output;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i].rfind(startStr, 0) == 0)
            {
                output.push_back(variableNames[i]);
            }
        }
        return output;
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

    template void GlobalMemoryHandler::FillVariable<double>(std::string, double);
    template void GlobalMemoryHandler::FillVariable<float> (std::string, float);
    template void GlobalMemoryHandler::FillVariable<int>   (std::string, int);
    template void GlobalMemoryHandler::FillVariable<bool>  (std::string, bool);
    template void GlobalMemoryHandler::FillVariable<char>  (std::string, char);
    template <typename vartype> void GlobalMemoryHandler::FillVariable(std::string name, vartype bValue)
    {
        vartype* x;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                x = (vartype*)hostBuffers[i];
                if (elementCount[i] < 0) __erkill("Cannot detect size out \"" + name + "\" for buffer filling.");
                for (int idx = 0; idx < elementCount[i]; idx++)
                {
                    x[idx] = bValue;
                }
                return;
            }
        }
        __erkill("Could not get appropriate global variable for \"" + name + "\"");
    }

    template void GlobalMemoryHandler::CopySingularBuffer<double>(std::string, double*);
    template void GlobalMemoryHandler::CopySingularBuffer<float> (std::string, float*);
    template void GlobalMemoryHandler::CopySingularBuffer<int>   (std::string, int*);
    template void GlobalMemoryHandler::CopySingularBuffer<bool>  (std::string, bool*);
    template void GlobalMemoryHandler::CopySingularBuffer<char>  (std::string, char*);
    template <typename vartype> void GlobalMemoryHandler::CopySingularBuffer(std::string name, vartype* buf)
    {
        vartype* x;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                x = (vartype*)hostBuffers[i];
                if (elementCount[i] < 0) WriteLine(1, "CANNOT COPY BUFFER WITH UNKOWN SIZE");
                for (int idx = 0; idx < elementCount[i]; idx++)
                {
                    buf[idx]=x[idx];
                }
                return;
            }
        }
        __erkill("Could not get appropriate global variable for \"" + name + "\"");
    }

    template void GlobalMemoryHandler::FillByFlag<double>(const int, double);
    template void GlobalMemoryHandler::FillByFlag<float> (const int, float);
    template void GlobalMemoryHandler::FillByFlag<int>   (const int, int);
    template void GlobalMemoryHandler::FillByFlag<bool>  (const int, bool);
    template void GlobalMemoryHandler::FillByFlag<char>  (const int, char);
    template <typename vartype> void GlobalMemoryHandler::FillByFlag(const int flag, vartype bValue)
    {
        vartype* x;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (HasFlag(manageModes[i], flag))
            {
                WriteLine(2, "Fill \"" +  variableNames[i] + "\" with " + std::to_string(elementCount[i]) + " elements, value " + std::to_string(bValue));
                x = (vartype*)hostBuffers[i];
                if (elementCount[i] < 0) WriteLine(1, "CANNOT FILL BUFFER WITH UNKOWN SIZE");
                for (int idx = 0; idx < elementCount[i]; idx++)
                {
                    x[idx] = bValue;
                }
            }
        }
    }

    void* GlobalMemoryHandler::GetVariable(std::string name, const int assertFlag)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                if (!HasFlag(manageModes[i], assertFlag)) __erkill("Variable \"" + name + "\" but does not have appropriate flag " + std::to_string(assertFlag));
                return hostBuffers[i];
            }
        }
        __erkill("Could not get appropriate global variable for \"" + name + "\"");
    }

    template void GlobalMemoryHandler::AddStaticVariable<double>(std::string, double**, double**, int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<float> (std::string, float**,  float**,  int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<int>   (std::string, int**,    int**,    int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<bool>  (std::string, bool**,   bool**,   int, int, const int);
    template void GlobalMemoryHandler::AddStaticVariable<char>  (std::string, char**,   char**,   int, int, const int);
    template <typename vartype> void GlobalMemoryHandler::AddStaticVariable(std::string name, vartype** hostSymbol, vartype** deviceSymbol,  int numPerRay, int dimension, const int manageMode)
    {
        //IMPORTANT: will need to add buffer mirroring for gpu variables
        size_t newVariableSize = numPerRay * dimension * localCpuPoints * sizeof(vartype);
        if (HasFlag(manageMode, bflag::serialHostUsage)) newVariableSize = numPerRay * dimension * sizeof(vartype);
        int newUpperAccessBound = newVariableSize / sizeof(vartype);

        elementCount[numGlobalVariables]           = dimension*numPerRay*localTotalPoints;
        userHasProvided[numGlobalVariables]        = false;
        isAllocated[numGlobalVariables]            = false;
        variableNames[numGlobalVariables]          = name;
        manageModes[numGlobalVariables]            = manageMode;
        bufferSizes[numGlobalVariables]            = newVariableSize;
        accessBounds[numGlobalVariables]           = newUpperAccessBound;
        hostSymbols[numGlobalVariables]            = (void**)hostSymbol;
        deviceSymbols[numGlobalVariables]          = (void**)deviceSymbol;
        doHostSymbolTransfer[numGlobalVariables]   = (hostSymbol != NULL);
        doDeviceSymbolTransfer[numGlobalVariables] = (deviceSymbol != NULL);
        // load balancing
        loadBalanceBuffer[numGlobalVariables]             = NULL;
        hasLoadBalanceBuffer[numGlobalVariables]          = false;
        loadBalanceBufferIsAllocated[numGlobalVariables]  = false;
        loadBalanceBufferSize[numGlobalVariables]         = 0;

        WriteLine(3, "Declaring static associated variable:");
        WriteLine(3, "name:       " + name);
        WriteLine(3, "managemode: " + std::to_string(manageMode));
        WriteLine(3, "numPerRay:  " + std::to_string(numPerRay));
        WriteLine(3, "dimension:  " + std::to_string(dimension));
        WriteLine(3, "size:       " + std::to_string(newVariableSize));

        if (HasFlag(manageMode, bflag::allocateNow))
        {
            // for averaging only really
            if (!HasFlag(manageMode, bflag::auxilary)) __erkill("A non-auxilary buffer has been marked for in-place allocation: \"" << name << "\"");
            WriteLine(3, "ALLOCATING IN-PLACE");
            hostBuffers[numGlobalVariables] = malloc(bufferSizes[numGlobalVariables]);
            isAllocated[numGlobalVariables] = true;
        }

        numGlobalVariables++;
    }

    bool GlobalMemoryHandler::VariableHasFlag(std::string name, int flag)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == name)
            {
                return HasFlag(manageModes[i], flag);
            }
        }
        __erkill("Could not get appropriate global variable for \"" + name + "\"");
    }

    void GlobalMemoryHandler::ApplyInitializationPolicies(void)
    {
        initializePoliciesWereApplied = true;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            int manageMode = manageModes[i];

            if (HasFlag(manageMode, bflag::userMustProvide) && !userHasProvided[i] && !Testing::testingMode) __erkill("Failed to find required user-provided variable \"" + variableNames[i] + "\"");

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
                if ((Parallel::pId == 0) && (settings.verboseLevel>3)) std::cout << "(ADDR) " << hostBuffers[i] << std::endl;
            }

            if (doHostSymbolTransfer[i])
            {
                *(hostSymbols[i]) = hostBuffers[i];
            }

        }
    }

    void GlobalMemoryHandler::ApplyRuntimePolicies(void)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {

        }
    }

    std::vector<std::string> GlobalMemoryHandler::GetVariablesByFlag(int flag)
    {
        std::vector<std::string> output;
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (HasFlag(manageModes[i], flag))
            {
                output.push_back(variableNames[i]);
            }
        }
        return output;
    }

    size_t GlobalMemoryHandler::GetVariableSize(std::string varname)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == varname)
            {
                return bufferSizes[i];
            }
        }
        __erkill("Could not get appropriate global variable for \"" + varname + "\"");
    }
    
    size_t GlobalMemoryHandler::GetVariableElemCount(std::string varname)
    {
        for (int i = 0; i < numGlobalVariables; i++)
        {
            if (variableNames[i] == varname)
            {
                return elementCount[i];
            }
        }
        __erkill("Could not get appropriate global variable for \"" + varname + "\"");
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
                WriteLine(2, "Freeing host-side buffer for \"" + variableNames[i] + "\"");
                free(hostBuffers[i]);
            }
        }
    }
    
    GlobalMemoryHandler::~GlobalMemoryHandler(void)
    {
        if (settings.loadBalance)
        {
            delete balancer;
            balancer = NULL;
        }
    }
}
