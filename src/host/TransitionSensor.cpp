#include "TransitionSensor.h"
#include "DebugTools.h"
#include "HyWall.h"
#include "ScreenOutput.h"
#include "CoreData.h"
namespace HyWall
{
    TransitionSensor::TransitionSensor(void) {}
    TransitionSensor::TransitionSensor(int sensorType_in, int pointNum_in)
    {
        sensorType = sensorType_in;
        pointNum = pointNum_in;
    }

    void TransitionSensor::DefineSensorVariables(void)
    {
        switch(sensorType)
        {
            case sensor::mettu18:
            {
                memory.AddStaticVariable<double>("aux:strain_rate",     &strain_rate,                NULL, 1, 1, bflag::auxilary | bflag::userMustProvide);
                memory.AddStaticVariable<double>("aux:strain_rate_avg", &strain_rate_avg,            NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:k",               &k,                          NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:k_avg",           &k_avg,                      NULL, 1, 1, bflag::auxilary | bflag::vtkOutput);
                memory.AddStaticVariable<double>("aux:rho_avg",         &rho_avg,                    NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:mu_avg",          &mu_avg,                     NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:u_avg",           &u_avg,                      NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:u_sq_avg",        &u_sq_avg,                   NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:v_avg",           &v_avg,                      NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:v_sq_avg",        &v_sq_avg,                   NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:w_avg",           &w_avg,                      NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:w_sq_avg",        &w_sq_avg,                   NULL, 1, 1, bflag::auxilary);
                memory.AddStaticVariable<double>("aux:sensor_val",      &sensor_val,                 NULL, 1, 1, bflag::auxilary | bflag::vtkOutput);
                memory.AddStaticVariable<double>("aux:sensorMult",      &HyCoreCPU::sensorMult,      NULL, 1, 1, bflag::auxilary);
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
                u   = (double*)memory.GetVariable("in:u");
                v   = (double*)memory.GetVariable("in:v");
                w   = (double*)memory.GetVariable("in:w");
                rho = (double*)memory.GetVariable("in:rho");
                mu  = (double*)memory.GetVariable("in:mu_lam");
                sensorMult = (double*)memory.GetVariable("aux:sensorMult");
                break;
            }
            default:
            {
                __erkill("Unknown sensorType with id " << sensorType);
            }
        }
    }

    void TransitionSensor::OnFirstSolve(void)
    {
        WriteLine(1, "Initialize Sensor (ID " + std::to_string(sensorType) + ")");
        switch (sensorType)
        {
            case sensor::mettu18:
            {
                mettu18_Init();
                break;
            }
            default:
            {
                __erkill("Unknown sensorType with id " << sensorType);
            }
        }
    }

    void TransitionSensor::mettu18_Init(void)
    {
        ZeroInit(k);
        ZeroInit(sensorMult);
        ZeroInit(k_avg);
        CopyAvg(u,           u_avg);
        CopyAvg(v,           v_avg);
        CopyAvg(w,           w_avg);
        CopyAvg(rho,         rho_avg);
        CopyAvg(strain_rate, strain_rate_avg);
        CopyAvg(mu,          mu_avg);
        CopySqAvg(u, u_sq_avg);
        CopySqAvg(v, v_sq_avg);
        CopySqAvg(w, w_sq_avg);
    }

    void TransitionSensor::mettu18_ComputeAverage(double* phibar, double* phi)
    {
        for (int i = 0; i < pointNum; i++)
        {
            double T =  1.41421356237 / (strain_rate[i]+1e-8);
            double dt_T = timeStep/T;
            phibar[i] = phibar[i]+dt_T*(phi[i]-phibar[i]);
        }
    }
    void TransitionSensor::mettu18_ComputeAverageSquare(double* phibar, double* phi)
    {
        for (int i = 0; i < pointNum; i++)
        {
            double T = 1.41421356237 / (strain_rate[i]+1e-8);
            double dt_T = timeStep/T;
            phibar[i] = phibar[i]+dt_T*((phi[i]*phi[i])-phibar[i]);
        }
    }

    void TransitionSensor::mettu18_ComputeSensorValues(void)
    {
        timeStep = settings.timeStep;

        mettu18_ComputeAverage(u_avg, u);
        mettu18_ComputeAverage(v_avg, v);
        mettu18_ComputeAverage(w_avg, w);
        mettu18_ComputeAverage(rho_avg, rho);
        mettu18_ComputeAverage(strain_rate_avg, strain_rate);
        mettu18_ComputeAverage(mu_avg, mu);
        mettu18_ComputeAverageSquare(u_sq_avg, u);
        mettu18_ComputeAverageSquare(v_sq_avg, v);
        mettu18_ComputeAverageSquare(w_sq_avg, w);

        for (int i = 0; i < pointNum; i++)
        {
            k[i] = 0;
            k[i] += u_sq_avg[i];
            k[i] += v_sq_avg[i];
            k[i] += w_sq_avg[i];
            k[i] -= u_avg[i]*u_avg[i];
            k[i] -= v_avg[i]*v_avg[i];
            k[i] -= w_avg[i]*w_avg[i];
        }
        mettu18_ComputeAverage(k_avg, k);

        for (int i = 0; i < pointNum; i++)
        {
            sensor_val[i] = (0.15 * rho_avg[i]*k_avg[i] / (mu_avg[i] * strain_rate_avg[i]))/settings.sensorThreshold;
            sensorMult[i] = (sensor_val[i]>1.0) ? 1.0 : 0.0;
        }
        double smin = Parallel::GlobalMin(sensor_val, pointNum);
        double smax = Parallel::GlobalMax(sensor_val, pointNum);
        WriteLine(1, "sensor min/max: " + std::to_string(smin) + ", " + std::to_string(smax));
    }

    void TransitionSensor::OnEverySolve(void)
    {
        switch (sensorType)
        {
            case sensor::mettu18:
            {
                mettu18_ComputeSensorValues();
                break;
            }
            default:
            {
                __erkill("Unknown sensorType with id " << sensorType);
            }
        }
    }

    void TransitionSensor::ZeroInit(double* a)
    {
        for (int i = 0; i < pointNum; i++) a[i] = 0;
    }

    void TransitionSensor::CopyAvg(double* b, double* a)
    {
        for (int i = 0; i < pointNum; i++) a[i] = b[i];
    }

    void TransitionSensor::CopySqAvg(double* b, double* a)
    {
        for (int i = 0; i < pointNum; i++) a[i] = b[i]*b[i];
    }
}
