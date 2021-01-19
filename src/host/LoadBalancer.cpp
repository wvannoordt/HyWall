#include "LoadBalancer.h"
#include "GlobalMemoryHandler.h"
#include "Parallel.h"
#include "ScreenOutput.h"
namespace HyWall
{
    LoadBalancer::LoadBalancer(GlobalMemoryHandler* mem_in)
    {
        mem = mem_in;
        int myTasksAfterLoadBalance;
        Balance(mem->localCpuPointsNative, &myTasksAfterLoadBalance);
    }
    
    void LoadBalancer::Balance(int numTasksIn, int* numTasksOut)
    {
        int numTasksBefore = numTasksIn;
        int* allTasks = new int[Parallel::pNum];
        Parallel::Allgather(&numTasksBefore, 1, allTasks, 1, HY_INT);
        size_t sum = 0;
        for (int i = 0; i < Parallel::pNum; i++) sum += allTasks[i];
        size_t meanTasks = sum/Parallel::pNum;
        size_t residual = sum%Parallel::pNum;
        WriteLine(4, "Mean tasks: " + std::to_string(meanTasks));
        WriteLine(4, "Residual:   " + std::to_string(residual));
        delete [] allTasks;
    }
    
    void LoadBalancer::DebugPrintTasks(int num)
    {
        Parallel::Sync();
        int* allTasks = new int[Parallel::pNum];
        Parallel::Allgather(&num, 1, allTasks, 1, HY_INT);
        for (int p = 0; p < Parallel::pNum; p++)
        {
            Parallel::Sync();
            if (p==Parallel::pId)
            {
                std::cout << "Proc " << p << std::endl;
                for (int i = 0; i < Parallel::pNum; i++)
                {
                    std::cout << allTasks[i] << std::endl;
                }
                std::cout << std::endl;
            }
            Parallel::Sync();
        }
        delete [] allTasks;
        Parallel::Sync();
    }
}