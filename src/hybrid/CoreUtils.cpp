#include "HybridComputing.h"
#include "DebugTools.h"
#include <iostream>
#include "CoreData.h"
#include "CoreUtils.h"
#include "Constants.h"
#include "Typedef.h"
#include <fstream>
#if(__cpu)
#include <cmath>
using std::pow;
using std::atan2;
using std::log;
using std::min;
#endif

namespace HyCore
{
    __common double getGrowthRate(double x0, double xn, double dx0, int Npts, double errorTol, int maxIts)
    {
        double Z = (xn - x0) / dx0;
        double epsilon = 100;
        double guess = 2.0;
        int k = 0;
        while (d_abs(epsilon) > errorTol && k++ < maxIts)
        {
            epsilon = pow(guess,(Npts - 1)) - Z*(guess) + Z - 1.0;
            guess = guess - (epsilon) / ((Npts-1)*pow(guess, (Npts - 2)) - Z);
        }
        if ((guess < 1.0) || (d_abs(epsilon) > errorTol)) __erkill("Could not generate grid for x0=" << x0 << ", xn=" << xn << ", dx0=" << dx0 << ". Try adjusting wall spacing.");
        return guess;
    }

    __common double SaBacksolve(double mutIn, double muIn, double rhoIn)
    {
        int k = 0;
        if (mutIn/muIn < 0.00004) return 0.0;
        double chi = mutIn / muIn;
        double epsilon = 100000;
        if (mutIn<1e-9) return 0;
        while (d_abs(epsilon) > 1e-8 && k < 1000)
        {
            double deriv = (4*CONST_SA_CV1_CUBED * pow(chi,3) + pow(chi,6))/(pow((CONST_SA_CV1_CUBED + pow(chi,3)), 2));
            epsilon = (((pow(chi,4))/(pow(chi,3) + CONST_SA_CV1_CUBED)) - (mutIn/muIn)) / deriv;
            chi = chi - 0.8*epsilon;
            k++;
        }
        if (d_abs(epsilon) > 1e-7)
        {
            __erkill("Divergent turbulent variable backsolve.");
        }
        return muIn*chi/rhoIn;
    }

    __common void ComputeProductionDestructionSA(double* P, double* D, double rho_in, double mu_in, double turb_in, double y_in, double dudy_in)
    {
        double omega = d_abs(dudy_in);
        double chi = rho_in*turb_in/mu_in;
        double chi3 = chi*chi*chi;
        double fv1 = chi3 / (chi3 + CONST_SA_CV1_CUBED);
        double fv2 = 1 - chi/(1+chi*fv1);
        double sbar = fv2*turb_in / (CONST_SA_KAPPA*CONST_SA_KAPPA*y_in*y_in);
        double shat = -1;

        shat = omega+sbar;
        shat = (shat<0.3*omega)?0.3*omega:shat;

        double ft2 = 0.0;
        double r = min(10.0, turb_in / (shat*CONST_SA_KAPPA*CONST_SA_KAPPA*y_in*y_in));
        double g = r+CONST_SA_CW2*(r*r*r*r*r*r - r);
        double g6 = g*g*g*g*g*g;
        double fw = g*pow(((1+CONST_SA_CW3_POW6)/(g6 + CONST_SA_CW3_POW6)),0.16666666666667);

        *P = CONST_SA_CB1*(1-ft2)*shat*turb_in;
        *D = (CONST_SA_CW1*fw - (CONST_SA_CB1/(CONST_SA_KAPPA*CONST_SA_KAPPA))*ft2)*(turb_in*turb_in/(y_in*y_in));
    }

    __common double GetUplusAllmaras(double yPlus)
    {
        return CONST_ALGEBRAIC_B
				+CONST_ALGEBRAIC_C1*log((yPlus+CONST_ALGEBRAIC_A1)*(yPlus+CONST_ALGEBRAIC_A1)+CONST_ALGEBRAIC_B1*CONST_ALGEBRAIC_B1)
		        -CONST_ALGEBRAIC_C2*log((yPlus+CONST_ALGEBRAIC_A2)*(yPlus+CONST_ALGEBRAIC_A2)+CONST_ALGEBRAIC_B2*CONST_ALGEBRAIC_B2)
	            -CONST_ALGEBRAIC_C3*atan2(CONST_ALGEBRAIC_B1, yPlus+CONST_ALGEBRAIC_A1)-CONST_ALGEBRAIC_C4*atan2(CONST_ALGEBRAIC_B2, yPlus+CONST_ALGEBRAIC_A2);
    }

    __common double MutSA(double turb, double rho, double mu)
    {
        double chi3 = turb*rho/mu;
        chi3 *= chi3*chi3;
        return rho*turb*chi3/(chi3 + CONST_SA_CV1_CUBED);
    }

    //validated
    __common void TDMASolve(tdsys& sys, const int num)
    {
        for (int i = 1; i < num; i++)
        {
            sys[TD_SUB][i-1] /= sys[TD_DIA][i-1]; //wi (store on subdiagonal)
            sys[TD_DIA][i]   -= sys[TD_SUB][i-1]*sys[TD_SUP][i-1]; //bi = bi - wi*c(i-1)
            sys[TD_RHS][i]   -= sys[TD_SUB][i-1]*sys[TD_RHS][i-1];
        }
        sys[TD_RHS][num-1] /= sys[TD_DIA][num-1];
        for (int i = num-2; i >= 0; i--) sys[TD_RHS][i] = (sys[TD_RHS][i] - sys[TD_SUP][i] * (sys[TD_RHS][i+1]))/sys[TD_DIA][i];
    }
}
