#include "HyWall.h"
#include "Averaging.h"
#include <string>
#include "ScreenOutput.h"
#include "DebugTools.h"

#define MAX_AVERAGING_VALS 1024
namespace HyWall
{
    double* coords[4];
    double* avgVars[MAX_AVERAGING_VALS];
    int numAvgVars;
    int numPoints;
    double runningTime;
    bool firstCall;
    void InitializeAveraging(void)
    {
        WriteLine(1, "Initialize averaging");
        numAvgVars = 0;
        runningTime = 0;
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
    }

    void ApplyAveragingTo(std::string variableName)
    {
        WriteLine(2, "Applying averaging to " + variableName);
        avgVars[3*numAvgVars] = (double*)memory.GetVariable(variableName);
        memory.AddStaticVariable<double>(variableName+"_avg1", avgVars+3*numAvgVars+1, NULL, 1, 1, bflag::auxilary | bflag::allocateNow);
        memory.AddStaticVariable<double>(variableName+"_avg2", avgVars+3*numAvgVars+2, NULL, 1, 1, bflag::auxilary | bflag::allocateNow);
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
                    phiAvg[i] =   alpha*phiAvg[i]   + beta*phi[i];
                    phiSqAvg[i] = alpha*phiSqAvg[i] + beta*phi[i]*phi[i];
                }
            }
        }
        firstCall = false;
    }
}
