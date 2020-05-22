#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H
#include <string>
#pragma HWPP include

namespace HyWall
{

    #pragma HWPP SerialIoStructStart UserSettings
    struct UserSettings
    {

        #pragma HWPP SerialIoMember allocMode amode::allCpu
        int allocMode;

        #pragma HWPP SerialIoMember errorTolerance 1e-6
        double errorTolerance;

        #pragma HWPP SerialIoMember gpuAllocationRatio 0.0
        double gpuAllocationRatio;

        #pragma HWPP SerialIoMember rayDim 30
        int rayDim;

        #pragma HWPP SerialIoMember asyncSolve false
        bool asyncSolve;

        #pragma HWPP SerialIoMember verboseLevel 1
    	int verboseLevel;

        #pragma HWPP SerialIoMember maxIterations 100
    	int maxIterations;

        #pragma HWPP SerialIoMember wallSpacing 1e-6
    	double wallSpacing;

        #pragma HWPP SerialIoMember wallTemperature 300
    	double wallTemperature;

        #pragma HWPP SerialIoMember adiabaticWall false
    	bool adiabaticWall;

        #pragma HWPP SerialIoMember fluidCp 1005.0
    	double fluidCp;

        #pragma HWPP SerialIoMember turbPradntl 0.9
    	double turbPradntl;

        #pragma HWPP SerialIoMember fluidPrandtl 0.72
    	double fluidPrandtl;

        #pragma HWPP SerialIoMember gasConstant 287.0
    	double gasConstant;

        #pragma HWPP SerialIoMember globalTotalNumWallPoints -999
    	int globalTotalNumWallPoints;

        #pragma HWPP SerialIoMember enableTransitionSensor false
    	bool enableTransitionSensor;

        #pragma HWPP SerialIoMember sensorType sensor::mettu18
    	int sensorType;

        #pragma HWPP SerialIoMember timeStep -1e50
    	double timeStep;

        #pragma HWPP SerialIoMember sensorThreshold 0.25
    	double sensorThreshold;

        #pragma HWPP SerialIoMember momentumEquationType HyCore::momentum::allmaras
    	int momentumEquationType;

        #pragma HWPP SerialIoMember turbulenceEquationType HyCore::turbulence::linear
    	int turbulenceEquationType;

        #pragma HWPP SerialIoMember energyEquationType HyCore::energy::linear
    	int energyEquationType;

        #pragma HWPP SerialIoMember momentumUnderRelaxationODE 0.8
    	double momentumUnderRelaxationODE;

        #pragma HWPP SerialIoMember turbulenceUnderRelaxationODE 0.6
    	double turbulenceUnderRelaxationODE;

        #pragma HWPP SerialIoMember energyUnderRelaxationODE 0.7
    	double energyUnderRelaxationODE;

        #pragma HWPP SerialIoMember solveSkip 1
        int solveSkip;

        #pragma HWPP SerialIoMember includeMomentumRhs false
        bool includeMomentumRhs;

        #pragma HWPP SerialIoMember alwaysReinitialize false
        bool alwaysReinitialize;

        #pragma HWPP SerialIoMember laminarSafetyVelocity -1.0
        double laminarSafetyVelocity;

        #pragma HWPP SerialIoMember IOEnable false
        bool IOEnable;

        #pragma HWPP SerialIoMember workingDirectory "hywall"
        std::string workingDirectory;

        #pragma HWPP SerialIoMember averageSolution false
        bool averageSolution;

        #pragma HWPP SerialIoMember averageOutputInterval 100
        int averageOutputInterval;

        #pragma HWPP SerialIoMember wallDataOutputInterval 100
        int wallDataOutputInterval;

        #pragma HWPP SerialIoMember isCompressible true
        bool isCompressible;

        #pragma HWPP SerialIoMember suthViscRef 1.45151376745308e-06
        double suthViscRef;

        #pragma HWPP SerialIoMember suthTRef 110.4
        double suthTRef;

        #pragma HWPP SerialIoMember viscousLaw HyCore::visclaw::constant
        int viscousLaw;
    };
    #pragma HWPP SerialIoStructEnd

}

#endif
