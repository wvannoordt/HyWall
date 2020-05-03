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
            template <typename vartype>
            void AddStaticVariable(std::string name, int num_per_ray, int dimension, const int managemode);
            void SetUserAssociatedVariable(std::string name, double* ptr);
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
            std::string variableNames[MAX_BUFFERS];
            int manageModes[MAX_BUFFERS];


    };
}
#endif
