#include "Variables.h"
#include "HyWall.h"
#include "DebugTools.h"

namespace HyWall
{
    void DefineInputVariables(void)
    {
        memory.AddStaticVariable<double>("in:p",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:u",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:v",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:w",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:T",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:turb",     1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:rho",      1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:mu_lam",   1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:distance", 1, 1, bflag::input | bflag::userMustProvide);
        __dumpwait(settings.verboseLevel);
    }

    void DefineAuxilaryVariables(void)
    {

    }

    void DefineMomentumVariables(void)
    {

    }

    void DefineEnergyVariables(void)
    {

    }

    void DefineTurbulentVariables(void)
    {

    }

    void DefineOutputVariables(void)
    {
        memory.AddStaticVariable<double>("out:vorticity", 1, 1, bflag::input | bflag::userCanProvide);
        memory.AddStaticVariable<double>("out:tau",       1, 1, bflag::input | bflag::userCanProvide);
        memory.AddStaticVariable<double>("out:heatflux",  1, 1, bflag::input | bflag::userCanProvide);
    }
}
