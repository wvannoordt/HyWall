#include "HyWall.h"
#include "FortranInterface.h"
#include <string>
#include <cstring>
#include "DebugTools.h"
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
    
    void hywall_getsolgradandsol_f(int* widx, int* solIdx, double* solOut, double* solGradOut)
    {
        GetSolutionAndGradient(*widx, *solIdx, solOut, solGradOut);
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
    
    void hywall_get_probe_pointer_f(char* name, int* namelen, double*& ptrOut, int* size)
    {
        char namebuf[120] = {0};
        int len = *namelen;
        memcpy(namebuf, name, len*sizeof(char));
        std::string strname(namebuf);
        ptrOut = (double*)memory.GetVariable(strname);
        *size = memory.localTotalPoints * settings.rayDim;
    }

    void hywall_copy_singular_buffer_f(char* name, int* namelen, double* buf)
    {
        char namebuf[120] = {0};
        int len = *namelen;
        memcpy(namebuf, name, len*sizeof(char));
        std::string strname(namebuf);
        memory.CopySingularBuffer<double>(strname, buf);
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

    void hywall_write_restart_file_f(int* nt_timestep)
    {
        WriteRestart(*nt_timestep);
    }

    void hywall_read_restart_file_f(int* nt_timestep)
    {
        ReadRestart(*nt_timestep);
    }
}
