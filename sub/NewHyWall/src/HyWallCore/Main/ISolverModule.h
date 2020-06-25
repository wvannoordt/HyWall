#ifndef SOLVER_MODULE_H
#define SOLVER_MODULE_H
#include <string>
namespace HyWall
{
    class SolverModule
    {
        public:
            SolverModule(void);
            virtual void Initialize(void);
            virtual void DefineInputs(void);
            virtual void OnFirstTimeStep(void);
            virtual void Solve(void);
            virtual void Finalize(void);
        protected:
            std::string solverName;
    }
}

#endif
