#ifndef HYWALL_LOAD_BALANCE_H
#define HYWALL_LOAD_BALANCE_H

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
    };
}

#endif