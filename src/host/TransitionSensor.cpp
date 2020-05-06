#include "TransitionSensor.h"
#include "DebugTools.h"
#include "HyWall.h"
namespace HyWall
{
    TransitionSensor::TransitionSensor(void) {}
    TransitionSensor::TransitionSensor(int sensorType_in)
    {
        sensorType = sensorType_in;
    }

    void TransitionSensor::DefineSensorVariables(void)
    {
        switch(sensorType)
        {
            case sensor::mettu18:
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
                break;
            }
            default:
            {
                __erkill("Unknown sensorType with id " << sensorType);
            }
        }
    }

    void TransitionSensor::CopySymbols(void)
    {
        switch(sensorType)
        {
            case sensor::mettu18:
            {
                (double*)memory.GetVariable("aux:strain_rate");
                (double*)memory.GetVariable("aux:strain_rate_avg");
                (double*)memory.GetVariable("aux:k");
                (double*)memory.GetVariable("aux:k_avg");
                (double*)memory.GetVariable("aux:rho_avg");
                (double*)memory.GetVariable("aux:mu_avg");
                (double*)memory.GetVariable("aux:u_avg");
                (double*)memory.GetVariable("aux:u_sq_avg");
                (double*)memory.GetVariable("aux:v_avg");
                (double*)memory.GetVariable("aux:v_sq_avg");
                (double*)memory.GetVariable("aux:w_avg");
                (double*)memory.GetVariable("aux:w_sq_avg");
                (double*)memory.GetVariable("aux:sensor_val");
                break;
            }
            default:
            {
                __erkill("Unknown sensorType with id " << sensorType);
            }
        }
    }
}
