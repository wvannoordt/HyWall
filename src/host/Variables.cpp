#include "Variables.h"
#include "HyWall.h"
#include "DebugTools.h"

namespace HyWall
{
    void DefineInputVariables(void)
    {
        //Pnce GPU implementation is good, should ideally pass in host and device symbols and immediately copy when available.
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
            memory.AddStaticVariable<double>("aux:k_avg",           1, 1, bflag::auxilary | bflag::vtkOutput);
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
        memory.AddStaticVariable<double>("sol:u", settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:d", settings.rayDim, 1, bflag::solution);
        if (HyCore::MomentumHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:mom0", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom1", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom2", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom3", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }
    }

    void DefineEnergyVariables(void)
    {
        memory.AddStaticVariable<double>("sol:T",   settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:rho", settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:mu",  settings.rayDim, 1, bflag::solution);
        if (HyCore::EnergyHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:engy0", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy1", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy2", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy3", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }
    }

    void DefineTurbulentVariables(void)
    {
        memory.AddStaticVariable<double>("sol:turb",  settings.rayDim, 1, bflag::solution);
        if (HyCore::TurbulenceHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:turb0", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb1", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb2", settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb3", settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }
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
