!Sparsh was here
module HyWallF

    use, intrinsic :: iso_c_binding

	implicit none

    real (c_double), dimension (:), allocatable, target :: swapBuffer, swapBufferSingular
    integer                                             :: hRayDimGlob, numPointsGlob

    contains

    subroutine HyWallSetDomainSize(numPoints)

        use, intrinsic :: iso_c_binding
        implicit none
        integer, intent(in) :: numPoints
        interface
			subroutine hywall_setdomainsize_f(numPointsF) bind (c)
				use iso_c_binding
                integer (c_int), intent(in) :: numPointsF
			end subroutine hywall_setdomainsize_f
		end interface
        call hywall_setdomainsize_f(numPoints)
        if (allocated(swapBufferSingular)) deallocate(swapBufferSingular)
        allocate(swapBufferSingular(numPoints))
        numPointsGlob = numPoints
        
    end subroutine HyWallSetDomainSize
    
    
    subroutine HyWallGetSolGradAndSol(wallPointIdx, solIdx, solOut, solGradOut)
        !wallPointIdx = index of the face w.r.t surface triangulation
        !solIdx = index of solution away from wall
        !solOut = T, rho, mu, mu_t, u 
        !solGradOut = T, rho, mu, mu_t, u (gradients)
        use, intrinsic :: iso_c_binding
        implicit none
        integer, intent(in)  :: wallPointIdx, solIdx
        real*8,  intent(out) :: solOut(1:5), solGradOut(1:5)
        interface
			subroutine hywall_getsolgradandsol_f(wallPointIdxF, solIdxF, solOutF, solGradOutF) bind (c)
				use iso_c_binding
                integer (c_int),    intent(in)  :: wallPointIdxF
                integer (c_int),    intent(in)  :: solIdxF
                real    (c_double), intent(out) :: solOutF(1:5)
                real    (c_double), intent(out) :: solGradOutF(1:5)
			end subroutine hywall_getsolgradandsol_f
		end interface
        call hywall_getsolgradandsol_f(wallPointIdx, solIdx, solOut, solGradOut)

    end subroutine HyWallGetSolGradAndSol

    subroutine HyWallWriteRestart(nt_timestep)

        use, intrinsic :: iso_c_binding
        implicit none
        integer, intent(in) :: nt_timestep
        interface
			subroutine hywall_write_restart_file_f(nt_timestepF) bind (c)
				use iso_c_binding
                integer (c_int), intent(in) :: nt_timestepF
			end subroutine hywall_write_restart_file_f
		end interface
        call hywall_write_restart_file_f(nt_timestep)

    end subroutine HyWallWriteRestart

    subroutine HyWallReadRestart(nt_timestep)

        use, intrinsic :: iso_c_binding
        implicit none
        integer, intent(in) :: nt_timestep
        interface
			subroutine hywall_read_restart_file_f(nt_timestepF) bind (c)
				use iso_c_binding
                integer (c_int), intent(in) :: nt_timestepF
			end subroutine hywall_read_restart_file_f
		end interface
        call hywall_read_restart_file_f(nt_timestep)

    end subroutine HyWallReadRestart
    
    subroutine HyWallDumpInputState(dirname)

        use, intrinsic :: iso_c_binding
        implicit none
        character*(*),   intent(in)    :: dirname
        interface
			subroutine hywall_dump_input_state_f(dirnameF, lenDirnameF) bind (c)
				use iso_c_binding
                character (c_char), intent(in)  :: dirnameF
                integer   (c_int),  intent(in)  :: lenDirnameF
			end subroutine hywall_dump_input_state_f
		end interface
        call hywall_dump_input_state_f(dirname, len(dirname))

    end subroutine HyWallDumpInputState
    
    subroutine HyWallDumpOuputState(dirname)

        use, intrinsic :: iso_c_binding
        implicit none
        character*(*),   intent(in)    :: dirname
        interface
			subroutine hywall_dump_output_state_f(dirnameF, lenDirnameF) bind (c)
				use iso_c_binding
                character (c_char), intent(in)  :: dirnameF
                integer   (c_int),  intent(in)  :: lenDirnameF
			end subroutine hywall_dump_input_state_f
		end interface
        call hywall_dump_output_state_f(dirname, len(dirname))

    end subroutine HyWallDumpOuputState

    subroutine HyWallGetProbePointer(arrayName, pointerOut)

        use, intrinsic :: iso_c_binding
        implicit none
        character*(*),   intent(in)    :: arrayName
        type(c_ptr) :: cdoublearray
        real(c_double), pointer :: pointerOut(:)
        integer(c_int) :: sizeInt
        interface
            subroutine hywall_get_probe_pointer_f(nameF, lenF, ptrF, sizeF) bind (c)
                use iso_c_binding
                character (c_char), intent(in)  :: nameF
                integer   (c_int),  intent(in)  :: lenF
                type(c_ptr),        intent(out) :: ptrF
                integer   (c_int),  intent(in)  :: sizeF
            end subroutine hywall_get_probe_pointer_f
        end interface
        call hywall_get_probe_pointer_f(arrayName, len(trim(arrayName)), cdoublearray, sizeInt)
        call c_f_pointer(cdoublearray, pointerOut, [sizeInt])

    end subroutine HyWallGetProbePointer

    subroutine HyWallDefineProbeIndex(arrayName, idx)

        use, intrinsic :: iso_c_binding
        implicit none
        character*(*),   intent(in)    :: arrayName
        integer (c_int), intent(out)   :: idx
        interface
            subroutine hywall_define_probe_index_f(nameF, lenF, idxF) bind (c)
                use iso_c_binding
                character (c_char), intent(in)  :: nameF
                integer   (c_int),  intent(in)  :: lenF
                integer   (c_int),  intent(out) :: idxF
            end subroutine hywall_define_probe_index_f
        end interface
        call hywall_define_probe_index_f(arrayName, len(trim(arrayName)), idx)

    end subroutine HyWallDefineProbeIndex


    subroutine HyWallCopySingularBuffer(arrayName, array)

        use, intrinsic :: iso_c_binding
        implicit none
        character*(*),   intent(in)     :: arrayName
        real*8,          intent(inout)  :: array(:)
        integer                         :: idx
        interface
            subroutine hywall_copy_singular_buffer_f(nameF, lenF, arF) bind (c)
                use iso_c_binding
                character (c_char), intent(in)  :: nameF
                integer   (c_int),  intent(in)  :: lenF
                type (c_ptr), intent(in), value  :: arF
            end subroutine hywall_copy_singular_buffer_f
        end interface
        call hywall_copy_singular_buffer_f(arrayName, len(trim(arrayName)), c_loc(swapBufferSingular))
        do idx = 1,numPointsGlob
            array(idx) = swapBufferSingular(idx)
        end do

    end subroutine HyWallCopySingularBuffer


    subroutine HywallProbeSolution(probeIndex, solutionIndex, array)

        use, intrinsic :: iso_c_binding
        implicit none
        integer (c_int), intent(in)     :: probeIndex
        integer (c_int), intent(in)     :: solutionIndex
        real*8,          intent(inout)  :: array(1:hRayDimGlob)
        integer                         :: idx
        interface
            subroutine hywall_probe_solution_f(probeIndexF, solutionIndexF, arF) bind (c)
                use iso_c_binding
                integer   (c_int), intent(in)    :: probeIndexF
                integer   (c_int), intent(in)    :: solutionIndexF
                type (c_ptr), intent(in), value  :: arF
            end subroutine hywall_probe_solution_f
        end interface
        call hywall_probe_solution_f(probeIndex, solutionIndex, c_loc(swapBuffer))
        do idx = 1,hRayDimGlob
            array(idx) = swapBuffer(idx)
        end do

    end subroutine HywallProbeSolution



    subroutine HyWallAwait()

        use, intrinsic :: iso_c_binding
        implicit none
        interface
			subroutine hywall_await_f() bind (c)
				use iso_c_binding
			end subroutine hywall_await_f
		end interface
        call hywall_await_f()

    end subroutine HyWallAwait



    subroutine HyWallAllocate

        use, intrinsic :: iso_c_binding
        implicit none
        integer :: hRayDim
        interface
            subroutine hywall_allocate_f() bind (c)
                use iso_c_binding
            end subroutine hywall_allocate_f
        end interface
        interface
            subroutine hywall_getraydim_f(hRayDimF) bind (c)
                use iso_c_binding
                integer(c_int), intent(out) :: hRayDimF
            end subroutine hywall_getraydim_f
        end interface
        call hywall_allocate_f()
        call hywall_getraydim_f(hRayDim)
        hRayDimGlob = hRayDim
        if (allocated(swapBuffer)) deallocate(swapBuffer)
        allocate(swapBuffer(hRayDim))

    end subroutine HyWallAllocate



    subroutine HyWallSetTimeStep(timestep)

        use, intrinsic :: iso_c_binding
        implicit none
        real*8, intent(in) :: timestep
        interface
            subroutine hywall_settimestep_f(timestepF) bind (c)
                use iso_c_binding
                real (c_double), intent(in) :: timestepF
            end subroutine hywall_settimestep_f
        end interface
        call hywall_settimestep_f(timestep)

    end subroutine HyWallSetTimeStep



    subroutine HyWallSolve

        use, intrinsic :: iso_c_binding
        implicit none
        interface
            subroutine hywall_solve_f() bind (c)
                use iso_c_binding
            end subroutine hywall_solve_f
        end interface
        call hywall_solve_f()

    end subroutine HyWallSolve



    subroutine HyWallPassVariable(arrayName, arrayPointer)

        use, intrinsic :: iso_c_binding
		implicit none
		character*(*), intent(in)       :: arrayName
		type (c_ptr), intent(in)        :: arrayPointer
		interface
			subroutine hywall_passvariable_f(nameF, pointerF, lenF) bind (c)
				use iso_c_binding
				character (c_char), intent(in)  :: nameF
				type (c_ptr), intent(in), value :: pointerF
				integer (c_int), intent(in)     :: lenF
			end subroutine hywall_passvariable_f
		end interface
		call hywall_passvariable_f(arrayName, arrayPointer, len(trim(arrayName)))

	end subroutine HyWallPassVariable



    subroutine HyWallDefineVariables()

        use, intrinsic :: iso_c_binding
        implicit none
        interface
            subroutine hywall_definevariables_f() bind (c)
                use iso_c_binding
            end subroutine hywall_definevariables_f
        end interface
        call hywall_definevariables_f()

    end subroutine HyWallDefineVariables



    subroutine HyWallPassFlowfieldVariables(arrayPointer, memOffset)

        use, intrinsic :: iso_c_binding
		implicit none
		type (c_ptr),  intent(in)   :: arrayPointer
		integer,       intent(in)   :: memOffset
		interface
			subroutine hywall_passflowfieldvariables_f(pointerF, offsetF) bind (c)
				use iso_c_binding
				type (c_ptr), intent(in), value :: pointerF
				integer (c_int), intent(in)     :: offsetF
			end subroutine hywall_passflowfieldvariables_f
		end interface
		call hywall_passflowfieldvariables_f(arrayPointer, memOffset)

	end subroutine HyWallPassFlowfieldVariables

end module HyWallF
