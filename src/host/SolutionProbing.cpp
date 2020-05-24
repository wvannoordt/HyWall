#include "GlobalMemoryHandler.h"
#include "HyWall.h"
#include "HybridComputing.h"
#include <string>
#include "BufferFlags.h"
#include "Indexing.h"
#include "DebugTools.h"

#define MAX_VAR_PROB_IDX 1024

namespace HyWall
{
    double* solutionProbes[MAX_VAR_PROB_IDX];
    int numProbes;

    void InitProbeIndex(void)
    {
        numProbes = 0;
    }

    void DefineProbeIndex(std::string varName, int* probeIndexOut)
    {
        solutionProbes[numProbes] = (double*)memory.GetVariable(varName, bflag::solution);
        *probeIndexOut = numProbes;
        numProbes++;
    }

    void ProbeSolution(int probeIndex, int solutionIndex, double* buffer)
    {
        int N = settings.rayDim;
        double* toProbe = solutionProbes[probeIndex];
        for (int i = 0; i<N; i++) buffer[i] = elem(toProbe, solutionIndex, i);
    }
}
