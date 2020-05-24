#ifndef SOLUTION_PROBING_H
#define SOLUTION_PROBING_H

#include <string>

namespace HyWall
{
    void InitProbeIndex(void);
    void DefineProbeIndex(std::string varName, int* probeIndexOut);
    void ProbeSolution(int probeIndex, int solutionIndex, double* buffer);
}

#endif
