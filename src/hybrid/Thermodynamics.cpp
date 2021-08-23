#include "Thermodynamics.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "Indexing.h"
#include "EquationTypes.h"
#include "CoreUtils.h"
#include "ViscousLaws.h"
#include "DebugTools.h"
#if(___cpu)
#include <cmath>
using std::pow;
#endif

namespace HyCore
{
    __common double SutherlandViscosityRelation(double T)
    {
        double mu = settings.suthViscRef*pow(T,1.5)/(T+settings.suthTRef);
        return mu;
    }

    __common double PowerLawViscosityRelation(double T)
    {
         double mu = settings.suthViscRef*pow(T/settings.suthTRef,0.76);
         return mu;
    }  

    __common double DensityRelation(double P, double T)
    {
        double rho = P/(settings.gasConstant*T);
        return rho;
    }

    __common void EquationsOfState(const int widx)
    {
        if (settings.turbulenceEquationType == turbulence::ODE) for (int i = 0; i < N-1; i++) elem(mu_t, widx, i) = MutSA(elem(turb, widx, i), elem(rho, widx, i), elem(mu, widx, i));
        if (settings.isCompressible) for (int i = 0; i < N; i++) elem(rho, widx, i) = DensityRelation(elem(p_F, widx), elem(T, widx, i));
        switch (settings.viscousLaw)
        {
            case visclaw::sutherland:
            {
                for (int i = 0; i < N; i++) elem(mu, widx, i) = SutherlandViscosityRelation(elem(T, widx, i));
                break;
            }
            case visclaw::PowerLaw:
            {
                for (int i = 0; i < N; i++) elem(mu, widx, i) = PowerLawViscosityRelation(elem(T, widx, i));
                break;
            }            
        }
    }
}
