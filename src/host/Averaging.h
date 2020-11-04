#ifndef AVERAGE_H
#define AVERAGE_H
#include <string>
namespace HyWall
{
    void InitializeAveraging(void);
    void ApplyAveragingTo(std::string variableName);
    void ComputeAverages(void);
    void FinalizeAveraging(void);
    void SaveAveragesToFile(int solveNumber);
    void ApplyAveragingTo(std::string variableName, int dim);
}

#endif
