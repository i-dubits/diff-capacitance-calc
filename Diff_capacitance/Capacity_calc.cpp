// Calculation of differential capacitance
// based on the paper "On the field effect in thin films of semiconductors with Kane’s charge-carrier dispersion relation"

#include "stdafx.h"
#include "Capacity_calc.h"
#include <gsl/gsl_integration.h>
#include <gsl/gsl_spline.h>
#include <gsl_math.h>
#include <iostream>

// constuctor
Capacity_calculation::Capacity_calculation(double v_s_s, double abserr_r, double relerr_r, int max_iter_r, double fermi_level_l,
											Concentration_functions* conc_f, material_parameters* mat_par_r, Fermi_searching* f_search_temp_p, MainDlg *dlg_g, double abserr_conv_testt)
{
	curr_iter = 0;

	v_s = v_s_s;
	abserr = abserr_r;
	relerr = relerr_r;
	max_iter = max_iter_r;
	fermi_level = fermi_level_l;
	conc_func = conc_f;
	mat_par = mat_par_r;
	f_search_temp = f_search_temp_p;
	dlg = dlg_g;		// MFC class, which is needed for GUI (in particular for spinMessageLoop())

	if (abserr_conv_testt == -1)	// error of iteration procedure is set to -1 by defualt because of compatibility issues
	{
		abserr_conv_test = abserr;
	}
	else
	{
		abserr_conv_test = abserr_conv_testt;
	}
}

// dim class initialization
// the thermal wavelength of the electron is taken as the characteristic length 
void Capacity_calculation::dim_config (void)
{
	dim.set_charact_l_as_wave_l_n_0();
	dim.set_band_gap( dim.ev_to_joule( mat_par -> gap() ) ); 
	dim.set_n_t( mat_par -> imp_c(), mat_par -> perm() );	// set concentration of impurity 
	dim.set_n_n (mat_par -> emass(), mat_par -> perm() );	// set n_c
	dim.set_n_v (mat_par -> hmass(), mat_par -> perm() );	// set n_c
}

// dim class initialization
// characteristic length is set by user
void Capacity_calculation::dim_config (double length)
{
	dim.set_charact_l_n_0(length);
	dim.set_band_gap( dim.ev_to_joule( mat_par -> gap() ) ); 
	dim.set_n_t(mat_par->imp_c(), mat_par->perm());			// set concentration of impurity 
	dim.set_n_n(mat_par->emass(), mat_par->perm());			// set n_c
	dim.set_n_v(mat_par->hmass(), mat_par->perm());			// set n_c

}


// right-hand side of Poisson equation  (it is written in the form compatible with GSL)
double Capacity_calculation::second_member_pausson (double u, void * param)
{
	Capacity_calculation* cap_calc = static_cast<Capacity_calculation*>(param);
	
	return 2*( (cap_calc -> dim.n_c_dless_out() ) * ( cap_calc -> conc_func -> concentration(cap_calc -> ferm_lev_out() + u, cap_calc -> f_search_temp) ) - 
		( cap_calc -> dim.n_v_dless_out() ) * (cap_calc -> conc_func -> concentration(-(cap_calc -> ferm_lev_out()) - (cap_calc -> dim.band_gap_dless()) - u, cap_calc -> f_search_temp)) -
		(cap_calc -> dim.n_t_dless_out()) );
}

//  right-hand side of Poisson equation integration over potential
double Capacity_calculation::integrate_second_member_pausson(double u, void* param)
{
	Capacity_calculation* cap_calc = static_cast<Capacity_calculation*>(param);
	
	// standard integration procedure 
	double result;
	double result_int_abs_err;	// absolute error of integral calculation

	gsl_integration_workspace * w 
		= gsl_integration_workspace_alloc (30000);

	gsl_function F;
	F.function = &Capacity_calculation::second_member_pausson;
	F.params = cap_calc;

	
	gsl_integration_qag (&F, 0, u, cap_calc->abserr_out(), cap_calc->relerr_out(), 30000,
						6,  w, &result, &result_int_abs_err);

	dlg->spinMessageLoop();		// interaction with GUI
	gsl_integration_workspace_free (w);

	
	return result;

}

// integrand for h function
double Capacity_calculation::integrate_for_h_func (double u, void* param)
{
	Capacity_calculation* cap_calc = static_cast<Capacity_calculation*>(param);
	double res = 0;
	res = pow( cap_calc->v_l_deriv_squared_auxiliary + cap_calc->integrate_second_member_pausson(u, cap_calc), -0.5 ) -
		pow( cap_calc->v_l_deriv_squared_auxiliary + u*u, -0.5 );
	return res;
}

// h function (see the paper)
double Capacity_calculation::h_function (double v_l_deriv_squared)
{
	//Capacity_calculation* cap_calc = static_cast<Capacity_calculation*>(param);

	v_l_deriv_squared_auxiliary = v_l_deriv_squared;

	// standard integration procedure 
	// method:  adaptive integration of a function without particularities

	double result;
	double result_int_abs_err;	// absolute error of integral calculation

	gsl_integration_workspace * w 
		= gsl_integration_workspace_alloc (30000);

	gsl_function F;
	F.function = &integrate_for_h_func;
	F.params = this;

	gsl_integration_qag (&F, 0, v_s_out(), abserr_out(), relerr_out(), 30000,
						6,  w, &result, &result_int_abs_err);

	gsl_integration_workspace_free (w);

	dlg->spinMessageLoop();		// interaction with GUI
	//printf ("\nAbs err: %le", result_int_abs_err); 

	return result;
}

// test for convergence of a series of successive approximations 
// analogue of gsl_root_test_delta  from root finding GSL
// 0 - fault, 1 - success
int Capacity_calculation::convergence_test (double x_1, double x_0)
{
	return ( abs( x_1 - x_0) < (abserr_conv_test + relerr * x_1) );	
}

// iteration procedure (see the paper)
// input v_s; output v_l_deriv_squared; argument - initial approximation
double Capacity_calculation::iteration_proc (double v_l_deriv_squared_initial)
{
	double v_l_deriv_squared;
	int status;

	curr_iter = 0;

	do
	{
		v_l_deriv_squared = pow( v_s / (  sinh( ( mat_par->th() ) * GSL_SIGN(v_s) -
			h_function(v_l_deriv_squared_initial) ) ), 2);
		curr_iter++;
		status = convergence_test(v_l_deriv_squared, v_l_deriv_squared_initial);
		if (v_l_deriv_squared <= 1e-10)	// small number for stability. as usual... 
		{
			break;
		}
		v_l_deriv_squared_initial = v_l_deriv_squared;

		dlg->spinMessageLoop();		// interaction with GUI
	}
	while (status != 1 && curr_iter != max_iter);

	if (curr_iter == max_iter)
	{
		printf ("\n\tERROR\n curr_iter == max_iter");
		return 0;
	}
	else
	{
		return v_l_deriv_squared;
	}
}


// function calculates v_s_deriv with known v_l_deriv_squared
// signs of derivative and potentail are choosen according to the following equation
// sgn(v_deriv) = -sgn(v_s) 
double Capacity_calculation::v_s_deriv_calc ( double v_l_deriv_squared)
{
	double v_s_deriv = 0;

	v_s_deriv = -GSL_SIGN(v_s) * sqrt(v_l_deriv_squared + integrate_second_member_pausson(v_s, this));
	return v_s_deriv;
}

// surface charge density calculation
double Capacity_calculation::suface_charge_density_calc ( double v_s_deriv, double v_l_deriv_squared)
{
	double v_l_deriv = 0;

	v_l_deriv = sqrt(v_l_deriv_squared) * (-GSL_SIGN(v_s));
	return v_s_deriv - v_l_deriv;
}

// capacity calculation
	// input: vector v_s
	// output: vector ñ;
vector<double> Capacity_calculation::capacity_calculation (vector<double> v_s_vector)
{
	vector<double> capacity_vector;
	vector<double> suface_charge_density_vector;

	double v_l_deriv_squared_initial = (v_s/mat_par->th())*(v_s/mat_par->th());	// initial approximation
	double v_l_deriv_squared;
	double v_s_deriv;
	double suface_charge_density_temp;	// introduced for compactness of notation 
	double capacity_temp;				// introduced for compactness of notation 

	for (size_t i = 0; i < v_s_vector.size(); i ++)
	{
		set_v_s(v_s_vector[i]);	// set v_s

		dlg->setProgress(int(1.0/v_s_vector.size() * (i + 1) * 100));		// calculation progress for GUI
		v_l_deriv_squared = iteration_proc (v_l_deriv_squared_initial);
		
		if (abs(v_s) > 5)
		{
			v_l_deriv_squared_initial = (mat_par->th() * mat_par->th() + 2*abs(v_s)) *
											(mat_par->th() * mat_par->th() + 2*abs(v_s)) / (4 * mat_par->th()*mat_par->th()) + 2;// from analytical equations for depletion and weak inversion (non-degenerate semiconductor). 
		}
		else
		{
			v_l_deriv_squared_initial = (mat_par->th() * mat_par->th() + 2*abs(v_s)) *
											(mat_par->th() * mat_par->th() + 2*abs(v_s)) / (4 * mat_par->th()*mat_par->th()) + 150;// from analytical equations for depletion and weak inversion (non-degenerate semiconductor)
		}

		v_s_deriv = v_s_deriv_calc (v_l_deriv_squared);

		suface_charge_density_temp = suface_charge_density_calc (v_s_deriv, v_l_deriv_squared);
		suface_charge_density_vector.push_back(suface_charge_density_temp);

		dlg->spinMessageLoop();		// interaction with GUI
		
	}

	// carry out interpolation and differentiation 
	// interpolate with cubic spline 
	// interpolation function needs double arrays
	double* v_s_array = new double[v_s_vector.size()];
	double* suface_charge_density_array = new double[v_s_vector.size()];

	// arrays initialization
	for (size_t i = 0; i <  v_s_vector.size(); i++)
	{
		v_s_array[i] = v_s_vector[i];
		suface_charge_density_array[i] = suface_charge_density_vector[i];
	}

	gsl_interp_accel *acc = gsl_interp_accel_alloc ();
	gsl_spline *spline = gsl_spline_alloc (gsl_interp_cspline, v_s_vector.size());
	gsl_spline_init (spline, v_s_array, suface_charge_density_array, v_s_vector.size());

	dlg->spinMessageLoop();		// interaction with GUI

	for (size_t i = 0; i <  v_s_vector.size(); i++)
	{
		capacity_temp = -gsl_spline_eval_deriv(spline, v_s_vector[i], acc); // minus sign stems from diff capacitance definition
		capacity_vector.push_back(capacity_temp);
	}

	// clean memory
	delete[] v_s_array;
	delete[] suface_charge_density_array;
	gsl_spline_free (spline);
    gsl_interp_accel_free (acc);

	return capacity_vector;
}
