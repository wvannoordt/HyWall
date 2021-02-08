#include "LoadBalancer.h"
#include "GlobalMemoryHandler.h"
#include "Parallel.h"
#include "ScreenOutput.h"
#include "ISort.h"
#include "Mat.h"
#include "HostUtils.h"
#include <algorithm>
#include "DebugTools.h"
namespace HyWall
{
    LoadBalancer::LoadBalancer(void)
    {
        
    }
    
    void LoadBalancer::Balance(int numTasksIn, int* numTasksOut)
    {
        int numTasksBefore = numTasksIn;
        std::vector<int> allTasks;
        std::vector<int> procs;
        int me = Parallel::pId;
        allTasks.resize(Parallel::pNum, 0);
        Parallel::Allgather(&numTasksBefore, 1, allTasks.data(), 1, HY_INT);
        for (int i = 0; i < Parallel::pNum; i++) procs.push_back(i);
        WriteLine(1, "Balancing Load");
        pattern = CreateLoadBalance(procs, allTasks);
        OptimizeForSparseness(pattern);
        *numTasksOut = numTasksIn;
        for (const auto& r:pattern[me]) *numTasksOut += r;
        WriteLine(1, "Balanced Load");
    }
    
    Mat<int> LoadBalancer::CreateLoadBalance(std::vector<int> procs, std::vector<int> numTasks)
    {
        if (numTasks.size()<1)
        {
            __erkill("Something weird is happening in the wall-model load-balancing. Please turn the load balancing off.");
        }
        std::vector<int> numTasksOrig = numTasks;
        int me = Parallel::pId;
        int nProc = procs.size();
        int minTasksPre = *std::min_element(numTasks.begin(), numTasks.end());
        int maxTasksPre = *std::max_element(numTasks.begin(), numTasks.end());
        size_t sum = 0;
        for (const auto t:numTasks) sum += t;
        int meanTasks = sum/procs.size();
        int residual = sum%procs.size();
        std::vector<int> allTasksToDonate;
        for (const auto i:numTasks) allTasksToDonate.push_back(i-meanTasks);
        std::vector<int> output;
        Mat<int> commPattern(procs.size(), procs.size());
        commPattern.fill(0);
        int maxIts = 1000;
        int tol = 10;
        for (int iter = 0; iter < maxIts; iter++)
        {
            ISort<int,int>(numTasks, procs);
            //small ... large
            int myTaskRank = std::distance(procs.begin(), std::find(procs.begin(), procs.end(), me));
            if (hy_abs(numTasks[numTasks.size()-1] - numTasks[0]) < tol)
            {
                break;
            }
            for (int i = 0; i < procs.size()/2; i++)
            {
                //careful... this is a little tricky!
                int receiver = procs[i];
                int donor = procs[nProc-i-1];
                int exchange = (meanTasks-numTasks[i])/6;
                commPattern(receiver, donor) += exchange;
                commPattern(donor, receiver) -= exchange;
            }
            std::vector<int> resultTasks;
            resultTasks.resize(nProc, 0);
            for (int i = 0; i < nProc; i++) resultTasks[procs[i]] = numTasksOrig[i];
            for (int i = 0; i < nProc; i++)
            {
                for (int j = 0; j < nProc; j++)
                {
                    resultTasks[procs[i]] += commPattern(i, j);
                }
            }
            numTasks = resultTasks;
        }
        int minTasksPost = *std::min_element(numTasks.begin(), numTasks.end());
        int maxTasksPost = *std::max_element(numTasks.begin(), numTasks.end());
        double minPercentPre = (double)minTasksPre/meanTasks;
        double maxPercentPre = (double)maxTasksPre/meanTasks;
        double minPercentPost = (double)minTasksPost/meanTasks;
        double maxPercentPost = (double)maxTasksPost/meanTasks;
        WriteLine(1, "Imbalanced spread: (" + std::to_string(100*minPercentPre)  + "%, " + std::to_string(100*maxPercentPre)  + "%)");
        WriteLine(1, "Balanced spread:   (" + std::to_string(100*minPercentPost) + "%, " + std::to_string(100*maxPercentPost) + "%)");
        return commPattern;
    }
    
    void LoadBalancer::OptimizeForSparseness(Mat<int>& nums)
    {
        // TODO
    }
    
    void LoadBalancer::SerialPrint(Mat<int>& nums)
    {
        if (0==Parallel::pId)
        {
            for (int i = 0; i < nums.size(0); i++)
            {
                for (int j = 0; j < nums.size(1); j++)
                {
                    std::cout << bufStr(nums(i, j), 6);
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
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
    void LoadBalancer::SerialPrint(std::vector<int> nums)
    {
        if (0==Parallel::pId)
        {
            std::cout << "---------" << std::endl;
            for(const auto& p:nums) std::cout << p << std::endl;
            std::cout << std::endl;
            std::cout << "---------" << std::endl;
        }
    }
    
    void LoadBalancer::DebugPrintTasks(std::vector<int> nums)
    {
        Parallel::Sync();
        for (int p = 0; p < Parallel::pNum; p++)
        {
            Parallel::Sync();
            if (p==Parallel::pId)
            {
                std::cout << "Proc " << p << std::endl;
                for(const auto& p:nums) std::cout << p << std::endl;
                std::cout << std::endl;
            }
            Parallel::Sync();
        }
        Parallel::Sync();
    }
    
    void LoadBalancer::DebugPrintTasks(Mat<int>& nums)
    {
        Parallel::Sync();
        for (int p = 0; p < Parallel::pNum; p++)
        {
            Parallel::Sync();
            if (p==Parallel::pId)
            {
                std::cout << "Proc " << p << std::endl;
                for (int i = 0; i < nums.size(0); i++)
                {
                    for (int j = 0; j < nums.size(1); j++)
                    {
                        std::cout << bufStr(nums(i, j), 6);
                    }
                    std::cout << std::endl;
                }
            }
            usleep(600);
            Parallel::Sync();
        }
        Parallel::Sync();
    }
}