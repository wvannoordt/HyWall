module HyWallF

    use, intrinsic :: iso_c_binding
	implicit none

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

    end subroutine HyWallSetDomainSize



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
