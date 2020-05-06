#include "mpi.h"

#ifndef PAR_H
#define PAR_H

namespace HyWall
{
    namespace Parallel
    {
        extern int pNum, pId, nameLength;
        extern char nodeName[MPI_MAX_PROCESSOR_NAME];
        extern MPI_Comm globalComm;
        void Initialize(MPI_Comm globalComm_in);
        void Finalize(void);
        double GlobalTotalAbs(double* ar, int num);
        double GlobalAverageAbs(double* ar, int num);
        double GlobalMaxAbs(double* ar, int num);
        double GlobalMax(double* ar, int num);
        double GlobalMin(double* ar, int num);
        void MarkActiveRanks(bool isActive_in);
    }
}

#endif
