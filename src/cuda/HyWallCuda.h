#ifndef HYWALL_CU_H
#define HYWALL_CU_H
namespace HyWall
{
    void CopyCudaSymbols(void);
    void InitGpuSolution(void);
    void BeginGpuSolution(void);
    void ComputeGpuSolution(void);
}
#endif
