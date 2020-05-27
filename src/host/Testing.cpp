#include "HyWall.h"
#include "Solver.h"
#include "EquationTypes.h"
#include "DebugTools.h"
#include "CoreData.h"
#include <cmath>
using std::cos;
using std::sin;
#include "Constants.h"
namespace HyWall
{
    namespace Testing
    {
        bool testingMode;
        int N;
        void TestMessage(const char* testname)
        {
            std::string p(testname);
            WriteLine(1, "Testing HyWall source in " + p);
        }

        void InitializeAsDummy(int rayDim_in, int numPoint_in)
        {
            InitializeInternalMPI(1);
            testingMode = true;
            SetDomainSize(numPoint_in);
            settings.rayDim = rayDim_in;
            N = rayDim_in;
        }

        void AllocateAsDummy(void)
        {
            DefineVariables();
            Allocate();
        }

        void MMSSetup(void)
        {
        	settings.momentumEquationType   = HyCore::momentum::ODE;
        	settings.turbulenceEquationType = HyCore::turbulence::ODE;
        	settings.energyEquationType     = HyCore::energy::ODE;
            settings.wallSpacing = 0.01;
            settings.verboseLevel = 1;
        }

        void RunMMS(double* momError, double* turbError, double* engyError)
        {
            double momErrorOutput  = 0.0;
            double turbErrorOutput = 0.0;
            double engyErrorOutput = 0.0;
            memory.FillByFlag<double>(bflag::solution | bflag::userMustProvide, 1.0);
            memory.FillVariable<double>("in:distance", 3*N*settings.wallSpacing);
            HyCoreCPU::BuildGrid(0);
            double* u = (double*)memory.GetVariable("sol:u");
            double* n = (double*)memory.GetVariable("sol:turb");
            double* T = (double*)memory.GetVariable("sol:T");
            double* y = (double*)memory.GetVariable("sol:d");

            for (int i = 0; i < N; i++) u[i] = sin(y[i]);
            for (int i = 0; i < N; i++) n[i] = cos(y[i]);
            for (int i = 0; i < N; i++) T[i] = 1.0+y[i]*y[i];

            HyCoreCPU::ComputeLhsRhsMomentumODE(0);
            HyCoreCPU::ComputeLhsRhsTurbulenceODE(0, 0.0);
            HyCoreCPU::ComputeLhsRhsEnergyODE(0);

            for (int i = 1; i < N-1; i++)
            {
                double analyticalRHS = -2.0*sin(y[i]);
                double delta = HyCoreCPU::momSystem[TD_RHS][i-1] - analyticalRHS;
                momErrorOutput += delta*delta;
            }

            for (int i = 1; i < N-1; i++)
            {
                double analyticalRHS = ((1.0+CONST_SA_CB2)*sin(y[i])*sin(y[i]) - cos(y[i]) - cos(y[i])*cos(y[i]))/CONST_SA_SIGMA;
                double delta = HyCoreCPU::turbSystem[TD_RHS][i-1] - analyticalRHS;
                turbErrorOutput += delta*delta;
            }

            double k = settings.fluidCp*((1.0 / settings.fluidPrandtl) + (1.0 / settings.turbPradntl));

            for (int i = 1; i < N-1; i++)
            {
                double analyticalRHS = 2.0*k - 2.0*sin(y[i])*sin(y[i]) + 2.0*cos(y[i])*cos(y[i]);
                double delta = HyCoreCPU::engySystem[TD_RHS][i-1] - analyticalRHS;
                engyErrorOutput += delta*delta;
            }

            *momError  = momErrorOutput/(N-2);
            *turbError = turbErrorOutput/(N-2);
            *engyError = engyErrorOutput/(N-2);
        }
    }
}
