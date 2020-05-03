#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include "AllocationModes.h"

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

        #pragma HWPP SerialIoMember globalTotalNumWallPoints -999
    	int globalTotalNumWallPoints;

        #pragma HWPP SerialIoMember enableTransitionSensor false
    	bool enableTransitionSensor;

        #pragma HWPP SerialIoMember sensorType sensor::mettu18
    	int sensorType;

        #pragma HWPP SerialIoMember timeStep -1e50
    	double timeStep;

        #pragma HWPP SerialIoMember transitionSensorThreshold 0.25
    	double transitionSensorThreshold;
    };
    #pragma HWPP SerialIoStructEnd

}

#endif
