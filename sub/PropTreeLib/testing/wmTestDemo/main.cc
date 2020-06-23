#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree input;

	bool useWallModel, outputWallModelData;
	int wallModelOutputDataInterval, forcingOffset, rayDim;
	double errorTolerance;
	std::string wallModelOutputDir;
	int momEquation;
	double* sampleXLocations;
	int numSampleXLocations;

	input["NavierStokes"]["WallModel"]["useWallModel"].MapTo(&useWallModel)
	= new PropTreeLib::Variables::PTLBoolean(false, "Flag to enable to wall model");

	input["NavierStokes"]["WallModel"]["outputWallModelData"].MapTo(&outputWallModelData)
	= new PropTreeLib::Variables::PTLBoolean(false, "Flag to enable wall model output");

	input["NavierStokes"]["WallModel"]["wallModelOutputDataInterval"].MapTo(&wallModelOutputDataInterval)
	= new PropTreeLib::Variables::PTLInteger(100, "Controls the interval at which wall model data is written");

	input["NavierStokes"]["WallModel"]["forcingOffset"].MapTo(&forcingOffset)
	= new PropTreeLib::Variables::PTLInteger(4, "Wall model sampling location (number of cells)");

	input["NavierStokes"]["WallModel"]["rayDim"].MapTo(&rayDim)
	= new PropTreeLib::Variables::PTLInteger(30, "Number of mesh points in wall-normal direction in the wall model");

	input["NavierStokes"]["WallModel"]["errorTolerance"].MapTo(&errorTolerance)
	= new PropTreeLib::Variables::PTLDouble(1e-6, "Generic error tolerance for iterative procedures in the wall model numerics");

	input["NavierStokes"]["WallModel"]["outputDir"].MapTo(&wallModelOutputDir)
	= new PropTreeLib::Variables::PTLString(".", "Location for wall model output data");

	input["NavierStokes"]["WallModel"]["Momentum"]["equation"].MapTo(&momEquation)
	= new PropTreeLib::Variables::PTLEnum("ODE", "ODE:allmaras:linear", "Specifies what kind of equation to solve for wall model momentum equation");

	input["NavierStokes"]["WallModel"]["SampleXLocations"].MapTo(&sampleXLocations, &numSampleXLocations)
	= new PropTreeLib::Variables::PTLDynamicDoubleArray("X-locations for inner-solution sampling with precision 1e-5");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
