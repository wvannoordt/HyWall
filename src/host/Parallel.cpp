#include "Parallel.h"
#include "mpi.h"

namespace HyWall
{
    namespace Parallel
    {
        int pNum, pId, nameLength;
        char nodeName[MPI_MAX_PROCESSOR_NAME];
        MPI_Comm globalComm;

        void Initialize(MPI_Comm globalComm_in)
        {
            MPI_Comm_dup(globalComm_in, &globalComm);
            MPI_Comm_size(globalComm, &pNum);
            MPI_Comm_rank(globalComm, &pId);
            MPI_Get_processor_name(nodeName, &nameLength);
        }

        void Finalize(void) {}
    }
}