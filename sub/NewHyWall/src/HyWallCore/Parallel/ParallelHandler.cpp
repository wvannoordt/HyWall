#include "ParallelHandler.h"
#include "ScreenOutput.h"
#include "TypeSwitch.h"
#include "DebugTools.hx"
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
            mpiInitFlag = -1;
            MPI_Initialized(&mpiInitFlag);
            if (!mpiInitFlag) MPI_Init(NULL, NULL);
            MPI_Comm_dup(MPI_COMM_WORLD, &comm);
            Rank();
        }
        
        void ParallelHandler::Rank(void)
        {
            MPI_Comm_size(comm, &pNum);
            MPI_Comm_rank(comm, &pId);
        }
        
        void ParallelHandler::CloseMPI(void)
        {
            MPI_Comm_free(&comm);
            int mpiCloseFlag = -1;
            MPI_Finalized(&mpiCloseFlag);
            if (!mpiCloseFlag & internalMPIHandling) MPI_Finalize();
        }
        

        ParallelHandler::~ParallelHandler(void)
        {
            CloseMPI();
            WriteLine(4, "Parallel close");
        }

        template void ParallelHandler::AllGather<int>   (const void*, int, void*, int);
        template void ParallelHandler::AllGather<char>  (const void*, int, void*, int);
        template void ParallelHandler::AllGather<bool>  (const void*, int, void*, int);
        template void ParallelHandler::AllGather<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::AllGather(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            MPI_Allgather(sendbuf, sendcount, mpitypeid(parType), recvbuf, recvcount, mpitypeid(parType), comm);
        }

        template void ParallelHandler::SumAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::SumAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::SumAllReduce(const void* sendbuf, int count, void* recvbuf, int recvcount)
        {
            MPI_Allreduce(sendbuf, recvbuf, count, mpitypeid(parType), MPI_SUM, comm);
        }

        template void ParallelHandler::SumAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::SumAbsAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::SumAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::SumAbsAllReduce not yet implemented");
        }

        template void ParallelHandler::AvgAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::AvgAllReduce<double>(const void*, int, void*, int);        
        template <typename parType> void ParallelHandler::AvgAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::AvgAllReduce not yet implemented");
        }

        template void ParallelHandler::AvgAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::AvgAbsAllReduce<double>(const void*, int, void*, int);        
        template <typename parType> void ParallelHandler::AvgAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::AvgAbsAllReduce not yet implemented");
        }

        template void ParallelHandler::MinAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MinAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MinAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::MinAllReduce not yet implemented");
        }

        template void ParallelHandler::MinAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MinAbsAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MinAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::MinAbsAllReduce not yet implemented");
        }

        template void ParallelHandler::MaxAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MaxAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MaxAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::MaxAllReduce not yet implemented");
        }

        template void ParallelHandler::MaxAbsAllReduce<int>   (const void*, int, void*, int);
        template void ParallelHandler::MaxAbsAllReduce<double>(const void*, int, void*, int);
        template <typename parType> void ParallelHandler::MaxAbsAllReduce(const void* sendbuf, int sendcount, void* recvbuf, int recvcount)
        {
            __erkill("ParallelHandler::MaxAbsAllReduce not yet implemented");
        }
    }
}
