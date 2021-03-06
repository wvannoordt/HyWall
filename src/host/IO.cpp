#include <string>
#include "IO.h"
#include <sys/stat.h>
#include "DebugTools.h"
#include "HyWall.h"
#include "Parallel.h"
#include <vector>
#include <cstring>
#include <unistd.h>
#include <string>
#include <fstream>
#define IONAMELEN 100
namespace HyWall
{
    namespace IO
    {
        void CreateDirectory(std::string dirname)
        {
            int st = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        bool FileExists(const std::string& name)
        {
            std::ifstream f(name.c_str());
            return f.good();
        }

        void WriteStateByFlags(std::string filename, int flags)
        {
            std::vector<std::string> vars = memory.GetVariablesByFlag(flags);
            int nvar = vars.size();
            char nameBuffer[IONAMELEN];
            MPI_File fh;
            MPI_Status st;
            MPI_File_open(MPI_COMM_WORLD, filename.c_str(), MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
            int writefactor = (Parallel::isRoot)?1:0;
            MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
            MPI_File_write_at(fh, 0, &(memory.globalTotalPoints), writefactor*1, MPI_INT, &st);
            MPI_File_write_at(fh, 1, &nvar, writefactor*1, MPI_INT, &st);
            MPI_File_set_view(fh, 2*sizeof(int), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
            int blockOffset = IONAMELEN+memory.globalTotalPoints*sizeof(double);
            int globalOffset = 0;
            int* pointNums = new int[Parallel::pNum];
            Parallel::Allgather(&(memory.localTotalPoints), 1, pointNums, 1, HY_INT);
            for (int i = 0; i < Parallel::pId; i++) globalOffset += pointNums[i];
            for (int i = 0; i < nvar; i++)
            {
                double* dataBuf = (double*)memory.GetVariable(vars[i]);
                restartFileContainedSolutionInit = restartFileContainedSolutionInit || memory.VariableHasFlag(vars[i], bflag::solution);
                memset(nameBuffer, 0, IONAMELEN*sizeof(char));
                vars[i].copy(nameBuffer, vars[i].length(), 0);
                MPI_File_write_at(fh, i*blockOffset, nameBuffer, writefactor*IONAMELEN, MPI_CHAR, &st);
                MPI_File_write_at(fh, i*blockOffset+IONAMELEN+globalOffset*sizeof(double), (char*)dataBuf, sizeof(double)*memory.localTotalPoints, MPI_CHAR, &st);
            }
            MPI_File_close(&fh);
            delete[] pointNums;
        }

        void ReadState(std::string filename)
        {
            if (!FileExists(filename)) __erkill("Cannot fild file " + filename);
            char nameBuffer[IONAMELEN];
            int nvar, dummy;
            MPI_File fh;
            MPI_Status st;
            MPI_File_open(MPI_COMM_WORLD, filename.c_str(), MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
            MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
            MPI_File_read_at(fh, 0, &(dummy), 1, MPI_INT, &st);
            if (dummy != memory.globalTotalPoints)
            {
                WriteLine(1, "WARNING: restart file defined on discretization. Ignoring restart data.");
                return;
            }
            MPI_File_read_at(fh, 1, &nvar, 1, MPI_INT, &st);
            MPI_File_set_view(fh, 2*sizeof(int), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
            int blockOffset = IONAMELEN+memory.globalTotalPoints*sizeof(double);
            int globalOffset = 0;
            int* pointNums = new int[Parallel::pNum];
            Parallel::Allgather(&(memory.localTotalPoints), 1, pointNums, 1, HY_INT);
            for (int i = 0; i < Parallel::pId; i++) globalOffset += pointNums[i];
            for (int i = 0; i < nvar; i++)
            {
                memset(nameBuffer, 0, IONAMELEN*sizeof(char));
                MPI_File_read_at(fh, i*blockOffset, nameBuffer, IONAMELEN, MPI_CHAR, &st);
                double* dataBuf = (double*)memory.GetVariable(std::string(nameBuffer));
                MPI_File_read_at(fh, i*blockOffset+IONAMELEN+globalOffset*sizeof(double), (char*)dataBuf, sizeof(double)*memory.localTotalPoints, MPI_CHAR, &st);
            }
            MPI_File_close(&fh);
            delete[] pointNums;
        }
    }
}
