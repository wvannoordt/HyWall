#include "HyWall.h"
#include "Averaging.h"
#include <string>
#include "ScreenOutput.h"
#include "DebugTools.h"
#include <cstring>
#include <vector>
#include <fstream>
#define NAME_BUF_LENGTH 32
#define MAX_AVERAGING_VALS 1024
namespace HyWall
{
    double* coords[4];
    double* avgVars[MAX_AVERAGING_VALS];
    int* pointNums;
    std::string avgVarNames[MAX_AVERAGING_VALS];
    int avgVarDims[MAX_AVERAGING_VALS];
    int numAvgVars;
    int numPoints;
    double runningTime;
    bool firstCall;
    int globalOffset;
    std::vector<double> lambda_avg, dT_dy_avg, lambda_dT_dy_avg;
    std::vector<double> u_avg, tau_avg, u_tau_avg, y_avg;
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
        coords[3] = (double*)memory.GetVariable("in:distance");
        ApplyAveragingTo("in:p");
        ApplyAveragingTo("in:u");
        ApplyAveragingTo("in:v");
        ApplyAveragingTo("in:w");
        ApplyAveragingTo("in:mu_t");
        ApplyAveragingTo("in:rho");
        ApplyAveragingTo("in:mu_lam");
        ApplyAveragingTo("out:tau");
        ApplyAveragingTo("out:heatflux");
        ApplyAveragingTo("sol:u", settings.rayDim);
        ApplyAveragingTo("sol:mu_t", settings.rayDim);
        ApplyAveragingTo("sol:rho", settings.rayDim);
        ApplyAveragingTo("sol:T", settings.rayDim);
        ApplyAveragingTo("sol:d", settings.rayDim);
        pointNums = new int[Parallel::pNum];
        Parallel::Allgather(&(memory.localTotalPoints), 1, pointNums, 1, HY_INT);
        for (int i = 0; i < Parallel::pId; i++) globalOffset += pointNums[i];
        
        lambda_avg.resize(settings.rayDim, 0.0);
        dT_dy_avg.resize(settings.rayDim, 0.0);
        lambda_dT_dy_avg.resize(settings.rayDim, 0.0);
        
        u_avg.resize(settings.rayDim, 0.0);
        tau_avg.resize(settings.rayDim, 0.0);
        u_tau_avg.resize(settings.rayDim, 0.0);
        y_avg.resize(settings.rayDim, 0.0);
    }

    void ApplyAveragingTo(std::string variableName)
    {
        ApplyAveragingTo(variableName, 1);
    }

    void ApplyAveragingTo(std::string variableName, int dim)
    {
        WriteLine(2, "Applying averaging to " + variableName);
        avgVars[3*numAvgVars] = (double*)memory.GetVariable(variableName);
        memory.AddStaticVariable<double>(variableName+"_avg1", avgVars+3*numAvgVars+1, NULL, dim, 1, bflag::auxilary | bflag::allocateNow | bflag::restorable);
        memory.AddStaticVariable<double>(variableName+"_avg2", avgVars+3*numAvgVars+2, NULL, dim, 1, bflag::auxilary | bflag::allocateNow | bflag::restorable);
        avgVarDims[numAvgVars] = dim;
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
            std::string varname(avgVarNames[3*n]);
            int dim = avgVarDims[n];
            WriteLine(3, "Averaging (n = " + std::to_string(n) + ")\"" + varname + "\" with dim " + std::to_string(dim));
            phi      = avgVars[3*n];
            phiAvg   = avgVars[3*n+1];
            phiSqAvg = avgVars[3*n+2];
            if (firstCall)
            {
                for (int i = 0; i < numPoints*dim; i++)
                {
                    phiAvg[i]   = phi[i];
                    phiSqAvg[i] = phi[i]*phi[i];
                }
            }
            else
            {
                for (int i = 0; i < numPoints*dim; i++)
                {
                    phiAvg[i]   = alpha*phiAvg[i]   + beta*phi[i];
                    phiSqAvg[i] = alpha*phiSqAvg[i] + beta*phi[i]*phi[i];
                }
            }
        }
        
        double* mu_buf   = (double*)memory.GetVariable("sol:mu");
        double* mu_t_buf = (double*)memory.GetVariable("sol:mu_t");
        double* u_buf    = (double*)memory.GetVariable("sol:u");
        double* T_buf    = (double*)memory.GetVariable("sol:T");
        double* y_buf    = (double*)memory.GetVariable("sol:d");
        
        double scal = 1.0/(double)(memory.localTotalPoints);
        for (int i = 0; i < memory.localTotalPoints; ++i)
        {
            std::size_t offset = i*settings.rayDim;
            auto inc = [&](int jh, int jl, int jm) ->void
            {
                double mu_loc   = mu_buf   [offset + jm];
                double mu_t_loc = mu_t_buf [offset + jm];
                double u_loc    = u_buf    [offset + jm];
                double y_loc    = y_buf    [offset + jm];
                double dy_loc   = y_buf[offset + jh] - y_buf[offset + jl];
                double dT_loc   = T_buf[offset + jh] - T_buf[offset + jl];
                double du_loc   = u_buf[offset + jh] - u_buf[offset + jl];
                double du_dy_loc = du_loc/dy_loc;
                double dT_dy_loc = dT_loc/dy_loc;
                double lambda = settings.fluidCp*(mu_loc/settings.fluidPrandtl + mu_t_loc/settings.turbPradntl);
                double tau    = mu_loc*du_dy_loc;
                
                lambda_avg[jm]       = alpha*lambda_avg[jm]       + beta*lambda;
                dT_dy_avg[jm]        = alpha*dT_dy_avg[jm]        + beta*dT_dy_loc;
                lambda_dT_dy_avg[jm] = alpha*lambda_dT_dy_avg[jm] + beta*lambda*dT_dy_loc;
                u_avg[jm]            = alpha*u_avg[jm]            + beta*u_loc;
                tau_avg[jm]          = alpha*tau_avg[jm]          + beta*tau;
                u_tau_avg[jm]        = alpha*u_tau_avg[jm]        + beta*u_loc*tau;
                y_avg[jm]            = alpha*y_avg[jm]            + beta*y_loc;
            };
            inc(1,0,0);
            for (int j = 1; j < settings.rayDim-1; j++)
            {
                inc(j+1,j-1,j);
            }
            inc(settings.rayDim-1,settings.rayDim-2,settings.rayDim-1);
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
        int totalVars = 3*numAvgVars;
        MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
        MPI_File_write_at(fh, 0, &(totalVars), writefactor*1, MPI_INT, &st);
        MPI_File_write_at(fh, 1, &(memory.globalTotalPoints), writefactor*1, MPI_INT, &st);
        MPI_File_set_view(fh, 2*sizeof(int), MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
        int blockOffset = NAME_BUF_LENGTH+memory.globalTotalPoints*sizeof(double);
        size_t currentPosition = 0;
        size_t currentLocalWriteSize = 0;
        size_t currentGlobalWriteSize = 0;
        for (int i = 0; i < numAvgVars; i++)
        {
            int dim = avgVarDims[i];
            currentLocalWriteSize = dim*sizeof(double)*memory.localTotalPoints;
            Parallel::Allreduce(&currentLocalWriteSize, &currentGlobalWriteSize, 1, HY_SIZE_T, HY_SUM);
            
            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+0].copy(nameBuffer, avgVarNames[3*i+0].length(), 0);
            MPI_File_write_at(fh, currentPosition, &dim, writefactor*sizeof(int), MPI_CHAR, &st);
            currentPosition += sizeof(int);
            MPI_File_write_at(fh, currentPosition, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            currentPosition += NAME_BUF_LENGTH;
            MPI_File_write_at(fh, currentPosition+dim*globalOffset*sizeof(double), (char*)avgVars[3*i+0], currentLocalWriteSize, MPI_CHAR, &st);
            currentPosition += currentGlobalWriteSize;

            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+1].copy(nameBuffer, avgVarNames[3*i+1].length(), 0);
            MPI_File_write_at(fh, currentPosition, &dim, writefactor*sizeof(int), MPI_CHAR, &st);
            currentPosition += sizeof(int);
            MPI_File_write_at(fh, currentPosition, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            currentPosition += NAME_BUF_LENGTH;
            MPI_File_write_at(fh,currentPosition+dim*globalOffset*sizeof(double), (char*)avgVars[3*i+1], currentLocalWriteSize, MPI_CHAR, &st);
            currentPosition += currentGlobalWriteSize;
            
            memset(nameBuffer, 0, NAME_BUF_LENGTH*sizeof(char));
            avgVarNames[3*i+2].copy(nameBuffer, avgVarNames[3*i+2].length(), 0);
            MPI_File_write_at(fh, currentPosition, &dim, writefactor*sizeof(int), MPI_CHAR, &st);
            currentPosition += sizeof(int);
            MPI_File_write_at(fh, currentPosition, nameBuffer, writefactor*NAME_BUF_LENGTH, MPI_CHAR, &st);
            currentPosition += NAME_BUF_LENGTH;
            MPI_File_write_at(fh, currentPosition+dim*globalOffset*sizeof(double), (char*)avgVars[3*i+2], currentLocalWriteSize, MPI_CHAR, &st);
            currentPosition += currentGlobalWriteSize;
        }
        MPI_File_close(&fh);
        
        std::vector<double> lambda_avg_root, dT_dy_avg_root, lambda_dT_dy_avg_root, u_avg_root, tau_avg_root, u_tau_avg_root, y_avg_root;
        
        lambda_avg_root.resize(settings.rayDim, 0.0);
        dT_dy_avg_root.resize(settings.rayDim, 0.0);
        lambda_dT_dy_avg_root.resize(settings.rayDim, 0.0);
        u_avg_root.resize(settings.rayDim, 0.0);
        tau_avg_root.resize(settings.rayDim, 0.0);
        u_tau_avg_root.resize(settings.rayDim, 0.0);
        y_avg_root.resize(settings.rayDim, 0.0);
        double loc_scale = (double)(memory.localTotalPoints)/(double)(memory.globalTotalPoints);
        for (auto& p: lambda_avg)       p *= loc_scale;
        for (auto& p: dT_dy_avg)        p *= loc_scale;
        for (auto& p: lambda_dT_dy_avg) p *= loc_scale;
        for (auto& p: u_avg)            p *= loc_scale;
        for (auto& p: tau_avg)          p *= loc_scale;
        for (auto& p: u_tau_avg)        p *= loc_scale;
        for (auto& p: y_avg)            p *= loc_scale;
        for (int k=0; k < lambda_avg.size(); ++k)
        {
            Parallel::Allreduce(&lambda_avg[k], &lambda_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&dT_dy_avg[k], &dT_dy_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&lambda_dT_dy_avg[k], &lambda_dT_dy_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&u_avg[k], &u_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&tau_avg[k], &tau_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&u_tau_avg[k], &u_tau_avg_root[k], 1, HY_DOUBLE, HY_SUM);
            Parallel::Allreduce(&y_avg[k], &y_avg_root[k], 1, HY_DOUBLE, HY_SUM);
        }
        for (auto& p: lambda_avg)       p /= loc_scale;
        for (auto& p: dT_dy_avg)        p /= loc_scale;
        for (auto& p: lambda_dT_dy_avg) p /= loc_scale;
        for (auto& p: u_avg)            p /= loc_scale;
        for (auto& p: tau_avg)          p /= loc_scale;
        for (auto& p: u_tau_avg)        p /= loc_scale;
        for (auto& p: y_avg)            p /= loc_scale;
        
        if (Parallel::isRoot)
        {
            std::ofstream myfile ("unsteady-energy-terms.csv");
            for (int k = 0; k < settings.rayDim; ++k)
            {
                myfile << y_avg_root[k]            << ",";
                myfile << lambda_avg_root[k]       << ",";
                myfile << dT_dy_avg_root[k]        << ",";
                myfile << lambda_dT_dy_avg_root[k] << ",";
                myfile << u_avg_root[k]            << ",";
                myfile << tau_avg_root[k]          << ",";
                myfile << u_tau_avg_root[k]        << "\n";
            }
        }
    }

    void FinalizeAveraging(void)
    {
        delete [] pointNums;
    }
}
