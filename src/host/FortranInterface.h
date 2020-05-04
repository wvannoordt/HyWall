#ifndef FORT_INTER_H
#define FORT_INTER_H

namespace HyWall
{
    extern "C"
    {
        void hywall_setdomainsize_f(int* numWallPoints_in);
        void hywall_passvariable_f(char* name, double* ptr, int* nameLen);
        void hywall_passflowfieldvariables_f(double* arrayPointer, int* memOffset);
        void hywall_definevariables_f(void);
        void hywall_allocate_f(void);
        void hywall_solve_f(void);
        void hywall_settimestep_f(double* timeStep);
    }
}

#endif
