#include "Parallel.h"
#include "mpi.h"
#include "DebugTools.h"
#include "HybridComputing.h"
#include "CoreUtils.h"

namespace HyWall
{
    namespace Parallel
    {
        int pNum, pId, nameLength;
        char nodeName[MPI_MAX_PROCESSOR_NAME];
        MPI_Comm globalComm;
        MPI_Comm wallpointComm;
        bool isActive;
        int activeNumLocal;
        int activeNumGlobal;

        void Initialize(MPI_Comm globalComm_in)
        {
            MPI_Comm_dup(globalComm_in, &globalComm);
            MPI_Comm_size(globalComm, &pNum);
            MPI_Comm_rank(globalComm, &pId);
            MPI_Get_processor_name(nodeName, &nameLength);
        }

        double GlobalAverageAbs(double* ar, int num)
        {
            double globalTotal;
            double total = 0.0;
            for (int i = 0; i < num; i++)
            {
                total += d_abs(ar[i]);
            }
            if (num > 0) total /= num;
            MPI_Allreduce(&total, &globalTotal, 1, MPI_DOUBLE, MPI_SUM, globalComm);
            return globalTotal/activeNumGlobal;
        }

        double GlobalTotalAbs(double* ar, int num)
        {
            double globalTotal;
            double total = 0.0;
            for (int i = 0; i < num; i++)
            {
                total += d_abs(ar[i]);
            }

            MPI_Allreduce(&total, &globalTotal, 1, MPI_DOUBLE, MPI_SUM, globalComm);
            return globalTotal;
        }

        double GlobalSum(double* ar, int num)
        {
            double glob_sum;
            double loc_sum = 0;
            if (num > 0)
            {
                for (int i = 0; i < num; i++) loc_sum += ar[i];
            }
            MPI_Allreduce(&loc_sum, &glob_sum, 1, MPI_DOUBLE, MPI_SUM, globalComm);
            return glob_sum;
        }

        void GlobalBoundsProduct(double* ar1, double* ar2, int num, double* minVal, double* maxVal)
        {
            double glob_min, glob_max;
            double loc_min = 1e100;
            double loc_max = -1e100;
            if (num > 0)
            {
                loc_min = ar1[0]*ar2[0];
                loc_max = ar1[0]*ar2[0];
                for (int i = 0; i < num; i++)
                {
                    double el = ar1[i]*ar2[i];
                    loc_min = (el<loc_min)?el:loc_min;
                    loc_min = (el>loc_max)?el:loc_max;
                }
            }

            MPI_Allreduce(&loc_min, &glob_min, 1, MPI_DOUBLE, MPI_MIN, globalComm);
            MPI_Allreduce(&loc_max, &glob_max, 1, MPI_DOUBLE, MPI_MAX, globalComm);
        }

        double GlobalMin(double* ar, int num)
        {
            double glob_min;
            double loc_min = 1e100;
            if (num > 0)
            {
                loc_min = ar[0];
                for (int i = 0; i < num; i++) loc_min = (ar[i]<loc_min)?ar[i]:loc_min;
            }

            MPI_Allreduce(&loc_min, &glob_min, 1, MPI_DOUBLE, MPI_MIN, globalComm);
            return glob_min;
        }

        double GlobalMax(double* ar, int num)
        {
            double glob_max;
            double loc_max = -1e100;
            if (num > 0)
            {
                loc_max = ar[0];
                for (int i = 0; i < num; i++) loc_max = (ar[i]>loc_max)?ar[i]:loc_max;
            }
            MPI_Allreduce(&loc_max, &glob_max, 1, MPI_DOUBLE, MPI_MAX, globalComm);
            return glob_max;
        }

        double GlobalMaxAbs(double* ar, int num)
        {
            double glob_max;
            double loc_max = 0;
            if (num > 0)
            {
                loc_max = d_abs(ar[0]);
                for (int i = 0; i < num; i++) loc_max = (d_abs(ar[i])>loc_max)?d_abs(ar[i]):loc_max;
            }
            MPI_Allreduce(&loc_max, &glob_max, 1, MPI_DOUBLE, MPI_MAX, globalComm);
            return glob_max;
        }

        void MarkActiveRanks(bool isActive_in)
        {
            activeNumLocal = isActive_in?1:0;
            MPI_Allreduce(&activeNumLocal, &activeNumGlobal, 1, MPI_INT, MPI_SUM, globalComm);
        }

        void Finalize(void) {}

    }
}
