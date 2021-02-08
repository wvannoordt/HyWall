#include "Variables.h"
#include "HyWall.h"
#include "DebugTools.h"
#include "EquationTypes.h"
#include "HybridComputing.h"
#include "CoreData.h"

namespace HyWall
{
    void DefineInputVariables(void)
    {
        //Once GPU implementation is good, should ideally pass in host and device symbols and immediately copy when available.
        memory.AddStaticVariable<double>("in:x",        &(HyCoreCPU::x),        NULL, 1, 1, bflag::constInput | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:y",        &(HyCoreCPU::y),        NULL, 1, 1, bflag::constInput | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:z",        &(HyCoreCPU::z),        NULL, 1, 1, bflag::constInput | bflag::userMustProvide | bflag::loadBalanced);

        memory.AddStaticVariable<double>("in:p",        &(HyCoreCPU::p_F),      NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:u",        &(HyCoreCPU::u_F),      NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:v",        &(HyCoreCPU::v_F),      NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:w",        &(HyCoreCPU::w_F),      NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:T",        &(HyCoreCPU::T_F),      NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:mu_t",     &(HyCoreCPU::mu_t_F),   NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:rho",      &(HyCoreCPU::rho_F),    NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:mu_lam",   &(HyCoreCPU::mu_F),     NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        memory.AddStaticVariable<double>("in:distance", &(HyCoreCPU::distance), NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
    }

    void DefineAuxilaryVariables(void)
    {
        if (settings.enableTransitionSensor)
        {
            tSensor = TransitionSensor(settings.sensorType, memory.localTotalPoints);
            tSensor.DefineSensorVariables();
        }
    }

    void DefineMomentumVariables(void)
    {
        memory.AddStaticVariable<double>("sol:u", &(HyCoreCPU::u), NULL, settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:d", &(HyCoreCPU::d), NULL, settings.rayDim, 1, bflag::solution);
        if (HyCore::MomentumHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:mom0", &(HyCoreCPU::momSystem[TD_SUB]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom1", &(HyCoreCPU::momSystem[TD_DIA]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom2", &(HyCoreCPU::momSystem[TD_SUP]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:mom3", &(HyCoreCPU::momSystem[TD_RHS]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }

        memory.AddStaticVariable<double>("in:momRHS", &(HyCoreCPU::momBalancedRHS), NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
        if (settings.includeMomentumRhs)
        {
            memory.AddStaticVariable<double>("sol:u_SA",  &(HyCoreCPU::u_SA),           NULL, settings.rayDim, 1, bflag::solution);
        }
        memory.AddStaticVariable<double>("in:dpdx",   &(HyCoreCPU::dpdx),           NULL, 1, 1, bflag::input | bflag::userMustProvide | bflag::loadBalanced);
    }

    void DefineEnergyVariables(void)
    {
        memory.AddStaticVariable<double>("sol:T",   &(HyCoreCPU::T),   NULL, settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:rho", &(HyCoreCPU::rho), NULL, settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:mu",  &(HyCoreCPU::mu),  NULL, settings.rayDim, 1, bflag::solution);
        if (HyCore::EnergyHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:engy0", &(HyCoreCPU::engySystem[TD_SUB]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy1", &(HyCoreCPU::engySystem[TD_DIA]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy2", &(HyCoreCPU::engySystem[TD_SUP]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:engy3", &(HyCoreCPU::engySystem[TD_RHS]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }
    }

    void DefineTurbulentVariables(void)
    {
        memory.AddStaticVariable<double>("sol:turb",  &(HyCoreCPU::turb), NULL, settings.rayDim, 1, bflag::solution);
        memory.AddStaticVariable<double>("sol:mu_t",  &(HyCoreCPU::mu_t), NULL, settings.rayDim, 1, bflag::solution);

        if (HyCore::TurbulenceHasJacobian(&settings))
        {
            memory.AddStaticVariable<double>("jac:turb0", &(HyCoreCPU::turbSystem[TD_SUB]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb1", &(HyCoreCPU::turbSystem[TD_DIA]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb2", &(HyCoreCPU::turbSystem[TD_SUP]), NULL, settings.rayDim-3, 1, bflag::solution | bflag::serialHostUsage);
            memory.AddStaticVariable<double>("jac:turb3", &(HyCoreCPU::turbSystem[TD_RHS]), NULL, settings.rayDim-2, 1, bflag::solution | bflag::serialHostUsage);
        }
    }

    void DefineOutputVariables(void)
    {
        memory.AddStaticVariable<double>("out:vorticity",   &(HyCoreCPU::vorticity),    NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
        memory.AddStaticVariable<double>("out:tau",         &(HyCoreCPU::tau),          NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
        memory.AddStaticVariable<double>("out:heatflux",    &(HyCoreCPU::heatflux),     NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
        memory.AddStaticVariable<double>("out:error"   ,    &(HyCoreCPU::error),        NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
        memory.AddStaticVariable<double>("out:iterations",  &(HyCoreCPU::iterations),   NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
        memory.AddStaticVariable<double>("out:failurelevel",&(HyCoreCPU::failurelevel), NULL, 1, 1, bflag::input | bflag::userCanProvide | bflag::vtkOutput | bflag::loadBalanced);
    }

    bool HasFlag(int a, int flag)
    {
        return (a&flag) > 0;
    }
}
