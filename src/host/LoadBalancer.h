#ifndef HYWALL_LOAD_BALANCE_H
#define HYWALL_LOAD_BALANCE_H
#include <vector>
#include "Mat.h"
namespace HyWall
{
    class LoadBalancer
    {
        public:
            LoadBalancer(void);
            void Balance(int numTasksIn, int* numTasksOut);
        private:
            void DebugPrintTasks(int num);
            void DebugPrintTasks(std::vector<int> nums);
            void DebugPrintTasks(Mat<int>& nums);
            Mat<int> CreateLoadBalance(std::vector<int> procs, std::vector<int> numTasks);
            void SerialPrint(std::vector<int> nums);
            void SerialPrint(Mat<int>& nums);
            
            // [col] receives [row, col] tasks from [row] <-- trust this.
            Mat<int> pattern;
            void OptimizeForSparseness(Mat<int>& nums);
    };
}

#endif