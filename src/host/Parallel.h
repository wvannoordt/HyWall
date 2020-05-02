#include "mpi.h"

#ifndef PAR_H
#define PAR_H

namespace HyWall
{
    namespace Parallel
    {
        extern int pnum, pid, nameLength;
        extern char nodeName[MPI_MAX_PROCESSOR_NAME];
        extern MPI_Comm globalComm;
        void Initialize(MPI_Comm globalComm_in);
        void Finalize(void);
    }
}

#endif
