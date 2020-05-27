#ifndef TESTING_H
#define TESTING_H

namespace HyWall
{
    namespace Testing
    {
        extern bool testingMode;
        void TestMessage(const char* testname);
        void InitializeAsDummy(int rayDim_in, int numPoint_in);
        void AllocateAsDummy(void);
        void MMSSetup(void);
        void RunMMS(double* momError, double* turbError, double* engyError);
        double MMSTurbODE(void);
        double MMSEngyODE(void);
        double MMSMomODE(void);
    }
}


#endif
