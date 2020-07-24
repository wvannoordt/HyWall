#ifndef PAR_H
#define PAR_H
#include "mpi.h"
namespace HyWall
{
    namespace Parallel
    {
        class ParallelHandler
        {
            public:
                int pId, pNum;
                bool isRoot;
                std::string nodeName;
                ParallelHandler(void);
                ~ParallelHandler(void);
                template <typename parType> void AllGather(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void SumAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void SumAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void AvgAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void AvgAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void MinAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void MinAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void MaxAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
                template <typename parType> void MaxAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount);
            private:
                bool internalMPIHandling;
        };
    }
}

#endif
