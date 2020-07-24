#include "ParallelHandler.h"
#include "ScreenOutput.h"
namespace HyWall
{
    namespace Parallel
    {
        ParallelHandler::ParallelHandler(void)
        {
            pId = 0;
            pNum = -1;
            isRoot = false;
            nodeName = "[NO NODE]";
            internalMPIHandling = false;
        }

        ParallelHandler::~ParallelHandler(void)
        {
            WriteLine(4, "Parallel close");
        }

        template void ParallelHandler::AllGather<int>   (const void*, int, void*, int);
        template void ParallelHandler::AllGather<char>  (const void*, int, void*, int);
        template void ParallelHandler::AllGather<bool>  (const void*, int, void*, int);
        template void ParallelHandler::AllGather<double>(const void*, int, void*, int);
        template void ParallelHandler::AllGather<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::AllGather(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::SumAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::SumAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::SumAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::SumAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::SumAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::SumAbsAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::SumAbsAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::SumAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::AvgAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::AvgAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::AvgAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::AvgAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::AvgAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::AvgAbsAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::AvgAbsAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::AvgAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::MinAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MinAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::MinAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MinAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::MinAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MinAbsAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::MinAbsAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MinAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::MaxAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MaxAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::MaxAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MaxAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }

        template void ParallelHandler::MaxAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MaxAbsAllReduce<double>(const void*, int, void*, int);
        template void ParallelHandler::MaxAbsAllReduce<float> (const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MaxAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {

        }
    }
}
