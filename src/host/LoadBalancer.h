#ifndef HYWALL_LOAD_BALANCE_H
#define HYWALL_LOAD_BALANCE_H
#include <vector>
#include "Mat.h"
namespace HyWall
{
    class GlobalMemoryHandler;
    class LoadBalancer
    {
        public:
            LoadBalancer(GlobalMemoryHandler* mem_in);
        private:
            GlobalMemoryHandler* mem;
            void Balance(int numTasksIn, int* numTasksOut);
            void DebugPrintTasks(int num);
            void DebugPrintTasks(std::vector<int> nums);
            void DebugPrintTasks(Mat<int>& nums);
            Mat<int> CreateLoadBalance(std::vector<int> procs, std::vector<int> numTasks);
            void SerialPrint(std::vector<int> nums);
            void SerialPrint(Mat<int>& nums);
            Mat<int> pattern;
    };
}

#endif