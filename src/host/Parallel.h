#include "mpi.h"

#ifndef PAR_H
#define PAR_H

#define HY_INT MPI_INT
#define HY_DOUBLE MPI_DOUBLE
#define HY_SIZE_T MPI_LONG
#define HY_SUM MPI_SUM
#define HY_DATATYPE const MPI_Datatype
#define HY_OPERATION const MPI_Op

namespace HyWall
{
    namespace Parallel
    {
        extern int pNum, pId, nameLength;
        extern char nodeName[MPI_MAX_PROCESSOR_NAME];
        extern MPI_Comm globalComm;
        extern bool internalMPIHandling;
        extern bool isRoot;
        void Initialize(MPI_Comm globalComm_in);
        void Finalize(void);
        double GlobalTotalAbs(double* ar, int num);
        double GlobalAverageAbs(double* ar, int num);
        double GlobalMaxAbs(double* ar, int num);
        double GlobalMaxAbs(int* ar, int num);
        double GlobalMax(double* ar, int num);
        double GlobalMin(double* ar, int num);
        double GlobalSum(double* ar, int num);
        void Allgather(const void* sendbuf, int sendcount, void* recvbuf, int recvcount, HY_DATATYPE DTYPE);
        void Allreduce(const void *sendbuf, void *recvbuf, int count, HY_DATATYPE datatype, HY_OPERATION op);
        void GlobalBoundsProduct(double* ar1, double* ar2, int num, double* minVal, double* maxVal);
        void MarkActiveRanks(bool isActive_in);
        void Sync(void);
    }
}

#endif
