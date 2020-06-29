#include "HyWall.h"
#include "Averaging.h"
#include <string>
#include "ScreenOutput.h"
#include "DebugTools.h"
#include <cstring>
#define NAME_BUF_LENGTH 32
#define MAX_AVERAGING_VALS 1024
namespace HyWall
{
    double* coords[4];
    double* avgVars[MAX_AVERAGING_VALS];
    int* pointNums;
    std::string avgVarNames[MAX_AVERAGING_VALS];
    int numAvgVars;
    int numPoints;
    double runningTime;
    bool firstCall;
    int globalOffset;
    void InitializeAveraging(void)
    {
        WriteLine(1, "Initialize averaging");
        numAvgVars = 0;
        runningTime = 0;
        globalOffset = 0;
        firstCall = true;
        numPoints = memory.localTotalPoints;
        coords[0] = (double*)memory.GetVariable("in:x");
        coords[1] = (double*)memory.GetVariable("in:y");
        coords[2] = (double*)memory.GetVariable("in:z");
        coords[2] = (double*)memory.GetVariable("in:distance");
        ApplyAveragingTo("in:p");
        ApplyAveragingTo("in:u");
        ApplyAveragingTo("in:v");
        ApplyAveragingTo("in:w");
        ApplyAveragingTo("in:mu_t");
        ApplyAveragingTo("in:rho");
        ApplyAveragingTo("in:mu_lam");
        ApplyAveragingTo("out:tau");
        ApplyAveragingTo("out:heatflux");
        pointNums = new int[Parallel::pNum];
        Parallel::Allgather(&(memory.localTotalPoints), 1, pointNums, 1, HY_INT);
        for (int i = 0; i < Parallel::pId; i++) globalOffset += pointNums[i];
    }

    void ApplyAveragingTo(std::string variableName)
    {
        WriteLine(2, "Applying averaging to " + variableName);
        avgVars[3*numAvgVars] = (double*)memory.GetVariable(variableName);
        memory.AddStaticVariable<double>(variableName+"_avg1", avgVars+3*numAvgVars+1, NULL, 1, 1, bflag::auxilary | bflag::allocateNow | bflag::restorable);
        memory.AddStaticVariable<double>(variableName+"_avg2", avgVars+3*numAvgVars+2, NULL, 1, 1, bflag::auxilary | bflag::allocateNow | bflag::restorable);

        avgVarNames[3*numAvgVars]   = variableName;
        avgVarNames[3*numAvgVars+1] = variableName+"_avg1";
        avgVarNames[3*numAvgVars+2] = variableName+"_avg2";
        numAvgVars++;
    }

    void ComputeAverages(void)
    {
        runningTime += settings.timeStep;
        WriteLine(1, "Compute averages");
        double* phi;
        double* phiAvg;
        double* phiSqAvg;
        double beta = settings.timeStep / runningTime;
        double alpha = 1.0 - beta;
        for (int n = 0; n < numAvgVars; n++)
        {
            phi      = avgVars[3*n];
            phiAvg   = avgVars[3*n+1];
            phiSqAvg = avgVars[3*n+2];
            if (firstCall)
            {
                for (int i = 0; i < numPoints; i++)
                {
                    phiAvg[i]   = phi[i];
                    phiSqAvg[i] = phi[i]*phi[i];
                }
            }
            else
            {
                for (int i = 0; i < numPoints; i++)
                {
                    phiAvg[i]   = alpha*phiAvg[i]   + beta*phi[i];
                    phiSqAvg[i] = alpha*phiSqAvg[i] + beta*phi[i]*phi[i];
                }
            }
        }
        firstCall = false;
    }

    void SaveAveragesToFile(int solveNumber)
    {
        //crunch time. Wrap in parallel::() later.
        double* f = (double*)memory.GetVariable("in:p");
        char nameBuffer[NAME_BUF_LENGTH];
        WriteLine(1, "Outputting Averages");
        MPI_File fh;
        MPI_Status st;
        MPI_File_open(MPI_COMM_WORLD, "hywallAveraging.dat", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh);
        int headerSize = 3*NAME_BUF_LENGTH*numAvgVars+sizeof(int);
        int writefactor = (Parallel::isRoot)?1:0;
        MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
        MPI_File_write_at(fh, 0, &(memory.globalTotalPoints), writefactor*1, MPI_INT, &st);
        MPI_File_set_view(fh, sizeof(int), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
        int blockOffset = NAME_BUF_LENGTH+memory.globalTotalPoints*sizeof(double);
        for (int i = 0; i < numAvgVars; i++)
        {
            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+0].copy(nameBuffer, avgVarNames[3*i+0].length(), 0);
            MPI_File_write_at(fh, (3*i+0)*blockOffset, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            MPI_File_write_at(fh, (3*i+0)*blockOffset+NAME_BUF_LENGTH+globalOffset*sizeof(double), (char*)avgVars[3*i+0], sizeof(double)*memory.localTotalPoints, MPI_CHAR, &st);

            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+1].copy(nameBuffer, avgVarNames[3*i+1].length(), 0);
            MPI_File_write_at(fh, (3*i+1)*blockOffset, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            MPI_File_write_at(fh, (3*i+1)*blockOffset+NAME_BUF_LENGTH+globalOffset*sizeof(double), (char*)avgVars[3*i+1], sizeof(double)*memory.localTotalPoints, MPI_CHAR, &st);

            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+2].copy(nameBuffer, avgVarNames[3*i+2].length(), 0);
            MPI_File_write_at(fh, (3*i+2)*blockOffset, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            MPI_File_write_at(fh, (3*i+2)*blockOffset+NAME_BUF_LENGTH+globalOffset*sizeof(double), (char*)avgVars[3*i+2], sizeof(double)*memory.localTotalPoints, MPI_CHAR, &st);
        }
        MPI_File_close(&fh);

    }

    void FinalizeAveraging(void)
    {
        delete [] pointNums;
    }
}
