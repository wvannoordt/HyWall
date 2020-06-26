#ifndef SOLVER_MODULE_H
#define SOLVER_MODULE_H
#include <string>
#include "PropTreeLib.h"
namespace HyWall
{
    class SolverModule
    {
        public:

            SolverModule(void)
            {
                solverName = "[SOLVER::NONE]";
            }
            void SetInput(PropTreeLib::PropertyTree & mainInput_in)
            {
                mainInput = mainInput_in;
            }

            virtual void Initialize(void);
            virtual void DefineInputs(void);
            virtual void OnFirstTimeStep(void);
            virtual void Solve(void);
            virtual void Finalize(void);
            virtual void GetName(void);
        protected:
            std::string solverName;
            PropTreeLib::PropertyTree & mainInput;
    }
}

#endif
