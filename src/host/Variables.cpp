#include "Variables.h"
#include "HyWall.h"
#include "DebugTools.h"

namespace HyWall
{
    void DefineInputVariables(void)
    {
        memory.AddStaticVariable<double>("in:x",        1, 1, bflag::constInput | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:y",        1, 1, bflag::constInput | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:z",        1, 1, bflag::constInput | bflag::userMustProvide);

        memory.AddStaticVariable<double>("in:p",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:u",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:v",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:w",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:T",        1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:turb",     1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:rho",      1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:mu_lam",   1, 1, bflag::input | bflag::userMustProvide);
        memory.AddStaticVariable<double>("in:distance", 1, 1, bflag::input | bflag::userMustProvide);
    }

    void DefineAuxilaryVariables(void)
    {
        if (settings.enableTransitionSensor)
        {
            memory.AddStaticVariable<double>("aux:strain_rate",     1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:strain_rate_avg", 1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:k",               1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:k_avg",           1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:rho_avg",         1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:mu_avg",          1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:u_avg",           1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:u_sq_avg",        1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:v_avg",           1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:v_sq_avg",        1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:w_avg",           1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:w_sq_avg",        1, 1, bflag::auxilary);
            memory.AddStaticVariable<double>("aux:sensor_val",      1, 1, bflag::auxilary | bflag::vtkOutput);
        }
    }

    void DefineMomentumVariables(void)
    {
        memory.AddStaticVariable<double>("sol:u",     settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:d",     settings.rayDim, 1, bflag::solution);
    }

    void DefineEnergyVariables(void)
    {
        memory.AddStaticVariable<double>("sol:T",     settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:rho",   settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:mu",    settings.rayDim, 1, bflag::solution);
    }

    void DefineTurbulentVariables(void)
    {
        memory.AddStaticVariable<double>("sol:turb",  settings.rayDim, 1, bflag::solution);
    }

    void DefineOutputVariables(void)
    {
        memory.AddStaticVariable<double>("out:vorticity",  1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput);
        memory.AddStaticVariable<double>("out:tau",        1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput);
        memory.AddStaticVariable<double>("out:heatflux",   1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput);
        memory.AddStaticVariable<double>("out:error"   ,   1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput);
        memory.AddStaticVariable<double>("out:iterations", 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput);
    }

    bool HasFlag(int a, int flag)
    {
        return (a&flag) > 0;
    }
}
