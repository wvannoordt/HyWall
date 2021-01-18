#ifndef FORT_INTER_H
#define FORT_INTER_H

namespace HyWall
{
    extern "C"
    {
        void hywall_setdomainsize_f(int* numWallPoints_in);
        void hywall_passvariable_f(char* name, double* ptr, int* nameLen);
        void hywall_passflowfieldvariables_f(double* arrayPointer, int* memOffset);
        void hywall_definevariables_f(void);
        void hywall_allocate_f(void);
        void hywall_solve_f(void);
        void hywall_settimestep_f(double* timeStep);
        void hywall_await_f(void);
        void hywall_get_probe_pointer_f(char* name, int* namelen, double*& ptrOut, int* size);
        void hywall_define_probe_index_f(char* name, int* namelen, int* idxOut);
        void hywall_getraydim_f(int* output);
        void hywall_probe_solution_f(int* probeIndex, int* solutionIndex, double* buffer);
        void hywall_copy_singular_buffer_f(char* name, int* namelen, double* buf);
        void hywall_write_restart_file_f(int* nt_timestep);
        void hywall_read_restart_file_f(int* nt_timestep);
        void hywall_getsolgradandsol_f(int* widx, int* solIdx, double* solOut, double* solGradOut);
        void hywall_dump_input_state_f(char* dirname, int* lenDirname);
        void hywall_dump_output_state_f(char* dirname, int* lenDirname);
        void hywall_dump_partition_f(char* dirname, int* lenDirname);
    }
}

#endif
