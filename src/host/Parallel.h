#include "mpi.h"

#ifndef PAR_H
#define PAR_H

namespace HyWall
{
    namespace parallel
    {
        extern int pnum, pid, nameLength;
        extern char nodeName[MPI_MAX_PROCESSOR_NAME];
        extern MPI_Comm globalComm;
        void initialize(MPI_Comm globalComm_in);
        void finalize(void);
    }
}

#endif
