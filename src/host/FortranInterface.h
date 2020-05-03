#ifndef FORT_INTER_H
#define FORT_INTER_H

namespace HyWall
{
    extern "C"
    {
        void hywall_setdomainsize_f(int* numWallPoints_in);
        void hywall_passvariable_f(char* name, double* ptr, int* nameLen);
        void hywall_passflowfieldvariables_f(double* arrayPointer, int* memOffset);
    }
}

#endif
