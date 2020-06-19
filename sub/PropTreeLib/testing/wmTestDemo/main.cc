#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree input;

	bool useWallModel, outputWallModelData;
	int wallModelOutputDataInterval, forcingOffset, rayDim;
	double errorTolerance;

	input["NavierStokes"]["WallModel"]["useWallModel"].MapTo(&useWallModel)
	= new PropTreeLib::Variables::Boolean(false, "Flag to enable to wall model");

	input["NavierStokes"]["WallModel"]["outputWallModelData"].MapTo(&outputWallModelData)
	= new PropTreeLib::Variables::Boolean(false, "Flag to enable wall model output");

	input["NavierStokes"]["WallModel"]["wallModelOutputDataInterval"].MapTo(&wallModelOutputDataInterval)
	= new PropTreeLib::Variables::Integer(100, "Controls the interval at which wall model data is written");

	input["NavierStokes"]["WallModel"]["forcingOffset"].MapTo(&forcingOffset)
	= new PropTreeLib::Variables::Integer(4, "Wall model sampling location (number of cells)");

	input["NavierStokes"]["WallModel"]["rayDim"].MapTo(&rayDim)
	= new PropTreeLib::Variables::Integer(30, "Number of mesh points in wall-normal direction in the wall model");

	input["NavierStokes"]["WallModel"]["errorTolerance"].MapTo(&errorTolerance)
	= new PropTreeLib::Variables::Double(1e-6, "Generic error tolerance for iterative procedures in the wall model numerics");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	return 0;
}
