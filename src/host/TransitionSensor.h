#ifndef TRANSIT_SENS_H
#define TRANSIT_SENS_H

namespace HyWall
{
    class TransitionSensor
    {
        public:
            TransitionSensor(void);
            TransitionSensor(int sensorType_in, int pointNum_in);
            void DefineSensorVariables(void);
            void CopySymbols(void);
            void OnFirstSolve(void);
            void OnEverySolve(void);
        private:
            void ZeroInit(double* a);
            void CopyAvg(double* b, double* a);
            void CopySqAvg(double* b, double* a);
            void mettu18_ComputeSensorValues(void);
            void mettu18_ComputeAverage(double* phibar, double* phi);
            void mettu18_ComputeAverageSquare(double* phibar, double* phi);
            void mettu18_Init(void);
            int sensorType;
            int pointNum;
            double timeStep;
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

            double* u;
            double* v;
            double* w;
            double* rho;
            double* mu;
    };
}

#endif
