#ifndef TRANSIT_SENS_H
#define TRANSIT_SENS_H

namespace HyWall
{
    class TransitionSensor
    {
        public:
            TransitionSensor(void);
            TransitionSensor(int sensorType_in);
            void DefineSensorVariables(void);
            void CopySymbols(void);
        private:
            int sensorType;
            double* strain_rate;
            double* strain_rate_avg;
            double* k;
            double* k_avg;
            double* rho_avg;
            double* mu_avg;
            double* u_avg;
            double* u_sq_avg;
            double* v_avg;
            double* v_sq_avg;
            double* w_avg;
            double* w_sq_avg;
            double* sensor_val;
    };
}

#endif
