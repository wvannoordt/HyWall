#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree input;

	input["userDefinedSection"]["haha"]["test1"] = "l";
	input["userDefinedSection"]["haha"]["test2"] = "HI";

	input.ReadInputFileToTreeData(filename);

	input.DebugPrint();
	/*proto["NavierStokes"]["WallModel"]["rayDim"] = inputVariableTypes.Integer("Number of ray points for wall model mesh");
	proto["NavierStokes"]["WallModel"]["useWallModel"] = inputVariableTypes.Boolean("Flag to turn on wall model");
	proto["NavierStokes"]["WallModel"]["GpuSettings"]["allocationRatio"]  = inputVariableTypes.DoubleBetween(0.0, 1.0, "Allocation ration for gpu settings", default = 0.1);
	proto["NavierStokes"]["WallModel"]["GpuSettings"]["convectiveScheme"] = inputVariableTypes.AnyOf("weno5fvs, description=WENO scheme, 5th order", "centralEEC", "Convective scheme for interior domain");*/
	return 0;
}
