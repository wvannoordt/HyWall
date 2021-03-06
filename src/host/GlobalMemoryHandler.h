#include "BufferFlags.h"
#include "Parallel.h"
#include "LoadBalancer.h"
#include <string>
#include <vector>
#ifndef GLOBAL_MEM_HAN_H
#define GLOBAL_MEM_HAN_H
namespace HyWall
{
    class GlobalMemoryHandler
    {
        public:
            GlobalMemoryHandler(void);
            ~GlobalMemoryHandler(void);
            void SetSize(int numPoints_in, int rayDim_in);
            template <typename vartype> void FillVariable(std::string name, vartype bValue);
            template <typename vartype> void AddStaticVariable(std::string name, vartype** hostSymbol, vartype** deviceSymbol,  int numPerRay, int dimension, const int manageMode);
            template <typename vartype> void FillByFlag(const int flag, vartype bValue);
            template <typename vartype> void CopySingularBuffer(std::string name, vartype* buf);
            void SetUserAssociatedVariable(std::string name, double* ptr);
            void* GetVariable(std::string name);
            void* GetVariable(std::string name, const int assertFlag);
            void ApplyInitializationPolicies(void);
            void ApplyRuntimePolicies(void);
            void ApplyFinalizationPolicies(void);
            std::vector<std::string> GetVariablesByFlag(int flag);
            std::vector<std::string> GetVarsByStartingString(std::string startStr);
            size_t GetVariableSize(std::string varname);
            size_t GetVariableElemCount(std::string varname);
            bool VariableHasFlag(std::string name, int flag);
            int localCpuPoints;
            int localGpuPoints;
            int localTotalPoints;
            int globalGpuPoints;
            int globalCpuPoints;
            int globalTotalPoints;
            int localCpuPointsNative;
            int localGpuPointsNative;
            int localTotalPointsNative;
        private:
            int rayDim;
            void* hostBuffers[MAX_BUFFERS];
            void*  loadBalanceBuffer[MAX_BUFFERS]; // stores the input / output information that results from load balancing
            bool   hasLoadBalanceBuffer[MAX_BUFFERS];
            bool   loadBalanceBufferIsAllocated[MAX_BUFFERS];
            size_t loadBalanceBufferSize[MAX_BUFFERS];
            
            void* deviceBuffers[MAX_BUFFERS];
            void** hostSymbols[MAX_BUFFERS];
            void** deviceSymbols[MAX_BUFFERS];
            bool doHostSymbolTransfer[MAX_BUFFERS];
            bool doDeviceSymbolTransfer[MAX_BUFFERS];
            std::string variableNames[MAX_BUFFERS];
            int manageModes[MAX_BUFFERS];
            bool isAllocated[MAX_BUFFERS];
            bool userHasProvided[MAX_BUFFERS];
            size_t bufferSizes[MAX_BUFFERS];
            int accessBounds[MAX_BUFFERS];
            int elementCount[MAX_BUFFERS];
            int numGlobalVariables;
            bool initializePoliciesWereApplied;
            LoadBalancer* balancer;
    };
}
#endif
