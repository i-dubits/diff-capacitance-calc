// functions which compute concentrations of holes and electrons 
// parabolic + Boltzmann, parabolic + Fermi-Dirac, relativistic  + Fermi-Dirac

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"

#include "Concentration_functions.h"
#include <gsl/gsl_sf_fermi_dirac.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_gamma.h>

#include "Fermi_searching.h" 
#include <iostream>


	// parabolic + Boltzmann
 double Par_boltzman_conc::concentration (double x, void* param)
{
	return exp(x);		
}
 double Par_boltzman_conc::concentration_deriv (double x, void* param)
{
	return exp(x);		
}

 void Par_boltzman_conc::concentration_fdf (double x, void* param, double* y, double *dy)
{
	*y = exp(x);
	*dy = exp(x);
}

	// parabolic + Fermi-Dirac
double Par_fermi_conc::concentration (double x, void* param)
{
	 Fermi_searching* ferm_s = static_cast<Fermi_searching*> (param);

	 return  gsl_sf_fermi_dirac_half (x);
}

double Par_fermi_conc::concentration_deriv (double x, void* param)
{
	 Fermi_searching* ferm_s = static_cast<Fermi_searching*> (param);

	 return  gsl_sf_fermi_dirac_mhalf (x);
}

void Par_fermi_conc::concentration_fdf (double x, void* param, 
										double* y, double *dy)
{
	*y = gsl_sf_fermi_dirac_half (x);
	*dy = gsl_sf_fermi_dirac_mhalf (x);
}

	// functions from Concentrations_utilities class (Kane + Fermi-Dirac approximation) 


double Concentrations_utilities::a_coeff (double k)
{
	return pow ( 1 + 15./4. * (k + 1) + 1./40. * (k + 1) * (k + 1), 0.5 );
}

double Concentrations_utilities::b_coeff (double k)
{
	return 1.8 + 0.61 * k;
}

double Concentrations_utilities::ñ_coeff (double k)
{
	return 2 + (2 - pow(2, 0.5) ) * pow (2 , -k);
}


double Concentrations_utilities::fermi_dirac_approx (double x, double k)
{
	return 1 / (  (  (k + 1) * pow(2 , k + 1) ) / 
		(pow (b_coeff(k) + x + 
		pow( pow ( abs(x - b_coeff(k)), ñ_coeff(k)) + 
		pow(a_coeff(k), ñ_coeff(k)), 1/(ñ_coeff(k)) ) , k + 1)) + exp(-x)/gsl_sf_gamma (k + 1) );
}
				// functions for Kane's dispersion relation

double Concentrations_utilities::integrand_for_conc_Kein_fermi (double x, void* param)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*>(param);

	return 2 / sqrt(M_PI) * ( sqrt(x) * sqrt ( 1 + x / (ferm_s->dim_out().band_gap_dless()) ) * 
		( 1 + 2 * x / (ferm_s->dim_out().band_gap_dless()) ) /
		( exp(x -  (ferm_s->fermi_s_opt_out()-> variable_for_conc_fun)) + 1) );
}

double Concentrations_utilities::integrand_for_conc_deriv_Kein_fermi (double x, void* param)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*>(param);

	return 2 / sqrt(M_PI) * ( sqrt(x) * sqrt ( 1 + x / (ferm_s->dim_out().band_gap_dless()) ) * 
		( 1 + 2 * x / (ferm_s->dim_out().band_gap_dless()) ) /
		( 4 * cosh( (x -  (ferm_s->fermi_s_opt_out()-> variable_for_conc_fun)) / 2 ) * 
		cosh( (x -  (ferm_s->fermi_s_opt_out()-> variable_for_conc_fun)) / 2 ) ) );
}


	// parabolic + Fermi-Dirac 
	// approximation is used during calculations

double Par_fermi_approx_conc::concentration (double x, void* param)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*> (param);

	return Concentrations_utilities::fermi_dirac_approx (x,  0.5);
}

double Par_fermi_approx_conc::concentration_deriv (double x, void* param)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*> (param);

	return Concentrations_utilities::fermi_dirac_approx (x,  -0.5);
}

void Par_fermi_approx_conc::concentration_fdf (double x, void* param, 
										double* y, double *dy)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*> (param);

	*y = Concentrations_utilities::fermi_dirac_approx (x,  0.5);
	*dy = Concentrations_utilities::fermi_dirac_approx (x,  -0.5);
}

	// Kane + Fermi-Dirac 
	// approximations are not applied

double Kein_fermi_conc::concentration (double x, void* param)
{

	Fermi_searching* ferm_s = static_cast<Fermi_searching*>(param);
	double result;
	double result_int_abs_err_TEMP;		

	ferm_s->fermi_s_opt_out()-> variable_for_conc_fun = x; 
	gsl_integration_workspace * w 
		= gsl_integration_workspace_alloc (30000);
	

	gsl_function F;
	F.function = &Concentrations_utilities::integrand_for_conc_Kein_fermi;
	F.params = ferm_s;

	gsl_integration_qagiu (&F, 0, 
		ferm_s->fermi_s_opt_out()->int_abs_err_out() , 
		ferm_s->fermi_s_opt_out()->int_rel_err_out(), 
		30000, w, &result, 
		&result_int_abs_err_TEMP
		/*ferm_s->fermi_s_opt_out()-> result_int_abs_err*/);
	gsl_integration_workspace_free (w);

	return result;

}

double Kein_fermi_conc::concentration_deriv (double x, void* param)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*>(param);
	double result;
	double result_int_abs_err_TEMP;		

	ferm_s->fermi_s_opt_out()-> variable_for_conc_fun = x; 
	gsl_integration_workspace * w 
		= gsl_integration_workspace_alloc (30000);
	

	gsl_function F;
	F.function = &Concentrations_utilities::integrand_for_conc_deriv_Kein_fermi;
	F.params = ferm_s;

	gsl_integration_qagiu (&F, 0, 
		ferm_s->fermi_s_opt_out()->int_abs_err_out() , 
		ferm_s->fermi_s_opt_out()->int_rel_err_out(), 
		30000, w, &result, 
		&result_int_abs_err_TEMP
		/*ferm_s->fermi_s_opt_out()-> result_int_abs_err*/);
	gsl_integration_workspace_free (w);

	return result;

}

void Kein_fermi_conc::concentration_fdf (double x, void* param,	
										 double* y, double *dy)
{
	Fermi_searching* ferm_s = static_cast<Fermi_searching*>(param);
	double result_int_abs_err_TEMP;
	double result_int_abs_err_TEMP_der;

	double result;

	ferm_s->fermi_s_opt_out()-> variable_for_conc_fun = x;
	gsl_integration_workspace * w 
		= gsl_integration_workspace_alloc (30000);
	

	gsl_function F;
	F.function = &Concentrations_utilities::integrand_for_conc_Kein_fermi;
	F.params = ferm_s;

	gsl_integration_qagiu (&F, 0, ferm_s->fermi_s_opt_out()->int_abs_err_out() , ferm_s->fermi_s_opt_out()->int_rel_err_out(), 30000, w, &result, &result_int_abs_err_TEMP /*ferm_s->fermi_s_opt_out()-> result_int_abs_err*/);
	gsl_integration_workspace_free (w);

	// concentration derivative 

	double result_deriv;

	ferm_s->fermi_s_opt_out()-> variable_for_conc_fun_deriv = x; 
	gsl_integration_workspace * w_deriv 
		= gsl_integration_workspace_alloc (30000);
	

	gsl_function F_deriv;
	F_deriv.function = &Concentrations_utilities::integrand_for_conc_deriv_Kein_fermi;
	F_deriv.params = ferm_s;

	gsl_integration_qagiu (&F_deriv, 0, ferm_s->fermi_s_opt_out()->int_abs_err_out() , ferm_s->fermi_s_opt_out()->int_rel_err_out(), 30000, w_deriv, &result_deriv, &result_int_abs_err_TEMP_der /*ferm_s->fermi_s_opt_out()-> result_int_abs_err*/);
	gsl_integration_workspace_free (w_deriv);

	// return results

	*y = result;
	*dy = result_deriv;


}