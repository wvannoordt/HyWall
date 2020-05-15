#include "BufferFlags.h"
#include "Parallel.h"
#include <string>
#ifndef GLOBAL_MEM_HAN_H
#define GLOBAL_MEM_HAN_H
namespace HyWall
{
    class GlobalMemoryHandler
    {
        public:
            GlobalMemoryHandler(void);
            void SetSize(int numPoints_in, int rayDim_in);
            template <typename vartype> void AddStaticVariable(std::string name, vartype** hostSymbol, vartype** deviceSymbol,  int numPerRay, int dimension, const int manageMode);
            void SetUserAssociatedVariable(std::string name, double* ptr);
            void* GetVariable(std::string name);
            void ApplyInitializationPolicies(void);
            void ApplyRuntimePolicies(void);
            void ApplyFinalizationPolicies(void);
            int localCpuPoints;
            int localGpuPoints;
            int localTotalPoints;
            int globalGpuPoints;
            int globalCpuPoints;
            int globalTotalPoints;
        private:
            int rayDim;
            void* hostBuffers[MAX_BUFFERS];
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
            int numGlobalVariables;
            bool initializePoliciesWereApplied;


    };
}
#endif
