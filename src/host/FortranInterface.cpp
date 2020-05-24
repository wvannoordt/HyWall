#include "HyWall.h"
#include "FortranInterface.h"
#include <string>
#include <cstring>
namespace HyWall
{
    void hywall_setdomainsize_f(int* numWallPoints_in)
    {
        SetDomainSize(*numWallPoints_in);
    }

    void hywall_passflowfieldvariables_f(double* arrayPointer, int* memOffset)
    {
        PassFlowfieldVariables(arrayPointer, *memOffset);
    }

    void hywall_definevariables_f(void)
    {
        DefineVariables();
    }

    void hywall_allocate_f(void)
    {
        Allocate();
    }

    void hywall_settimestep_f(double* timeStep)
    {
        SetTimeStep(*timeStep);
    }

    void hywall_getraydim_f(int* output)
    {
        *output = settings.rayDim;
    }

    void hywall_define_probe_index_f(char* name, int* namelen, int* idxOut)
    {
        char namebuf[120] = {0};
        int len = *namelen;
        memcpy(namebuf, name, len*sizeof(char));
        std::string strname(namebuf);
        DefineProbeIndex(strname, idxOut);
    }

    void hywall_probe_solution_f(int* probeIndex, int* solutionIndex, double* buffer)
    {
        ProbeSolution(*probeIndex, *solutionIndex, buffer);
    }

    void hywall_solve_f(void)
    {
        Solve();
    }

    void hywall_await_f(void)
    {
        Await();
    }

    void hywall_passvariable_f(char* name, double* ptr, int* nameLen)
    {
        char namebuf[120] = {0};
        int len = *nameLen;
        memcpy(namebuf, name, len*sizeof(char));
        std::string strname(namebuf);
        PassVariable(strname, ptr);
    }
}
