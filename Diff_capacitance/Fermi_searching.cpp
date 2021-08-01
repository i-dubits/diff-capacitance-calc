// Fermi level calculation

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>


#include "material_parameters.h"
#include "phys_const.h" 
#include "dimension.h"
#include "Concentration_functions.h"
#include "Fermi_searching.h" 


// constructor
Fermi_searching_options::Fermi_searching_options(double abs_err, double rel_err, long max_iter_r, double in_guess, double int_abs_err_r, double int_rel_err_r)
{
	absolute_err = abs_err;
	relative_err = rel_err;
	max_iter = max_iter_r;
	initial_guess = in_guess;
	int_abs_err	= int_abs_err_r;
	int_rel_err = int_rel_err_r;
	iter = 0;
	iter_status = 0;
}


// constructors

Fermi_searching::Fermi_searching( Concentration_functions* conc_func_c , Fermi_searching_options* fermi_opt_t, material_parameters* mat_par_r)
{			
	conc_func = conc_func_c;
	fermi_opt = fermi_opt_t;
	mat_par = mat_par_r;
	fermi_level = 0;
}


void Fermi_searching::dim_config (void)
{
	dim.set_charact_l_as_wave_l_n_0();
	dim.set_band_gap( dim.ev_to_joule( mat_par -> gap() ) ); 
	// mat_par -> impurity_conc и dim -> n_t это одна и таже величина!
	dim.set_n_t( mat_par -> imp_c(), mat_par -> perm() );	
	dim.set_n_с (mat_par -> emass(), mat_par -> perm() );	
	dim.set_n_v (mat_par -> hmass(), mat_par -> perm() );	
}


void Fermi_searching::dim_config (double length)
{
	dim.set_charact_l_n_0(length);
	dim.set_band_gap( dim.ev_to_joule( mat_par -> gap() ) ); 
	dim.set_n_t( mat_par -> imp_c(), mat_par -> perm() );	
	dim.set_n_с (mat_par -> emass(), mat_par -> perm() );	
	dim.set_n_v (mat_par -> hmass(), mat_par -> perm() );
}



// x - Fermi level
double Fermi_searching::electro_neutral_eq (double x, void* param)
{
	Fermi_searching* ferm_s = (Fermi_searching*) param;
	return ( ferm_s -> dim.n_c_dless_out() ) * ( ferm_s -> conc_func -> concentration(x, param) ) - 
		( ferm_s -> dim.n_v_dless_out() ) * ( ferm_s -> conc_func -> concentration(-x - (ferm_s -> dim.band_gap_dless()), param)) -
		(ferm_s -> dim.n_t_dless_out()); 
}

double Fermi_searching::electro_neutral_eq_deriv( double x, void* param )
{
	Fermi_searching* ferm_s = (Fermi_searching*) param;
	return (ferm_s -> dim.n_c_dless_out() ) * ( ferm_s -> conc_func->concentration_deriv(x, param) ) + 
		( ferm_s -> dim.n_v_dless_out() ) * (ferm_s -> conc_func->concentration_deriv(-x - (ferm_s -> dim.band_gap_dless()), param));
}

void Fermi_searching::electro_neutral_eq_fdf( double x, void* param, double* y, double *dy)
{
	Fermi_searching* ferm_s = (Fermi_searching*) param;
	*y = (ferm_s -> dim.n_c_dless_out() ) * ( ferm_s -> conc_func -> concentration(x, param) ) - 
		( ferm_s -> dim.n_v_dless_out() ) * ( ferm_s -> conc_func->concentration(-x - ( ferm_s -> dim.band_gap_dless()), param)) -
		( ferm_s -> dim.n_t_dless_out()); 
	*dy = ( ferm_s -> dim.n_c_dless_out() ) * ( ferm_s -> conc_func->concentration_deriv(x, param) ) + 
		( ferm_s -> dim.n_v_dless_out() ) * ( ferm_s -> conc_func->concentration_deriv(-x - (ferm_s -> dim.band_gap_dless()), param)); 

}	
// methods
double Fermi_searching::newton_method() // Newton method 
{
	int solve_status;		
	
	const gsl_root_fdfsolver_type *T;
	gsl_root_fdfsolver *s;
	gsl_function_fdf FDF;

	FDF.f = &electro_neutral_eq;
	FDF.df = &electro_neutral_eq_deriv;
	FDF.fdf = &electro_neutral_eq_fdf;
	FDF.params = this;

	T = gsl_root_fdfsolver_newton;		// Newton method
	s = gsl_root_fdfsolver_alloc (T);	// allocate memory
	gsl_root_fdfsolver_set (s, &FDF, fermi_opt->in_guess());	// solver initialization
	do
	{
		fermi_opt->add_one_iterate();
		fermi_opt->iter_status = gsl_root_fdfsolver_iterate (s);		// make step
		fermi_level = gsl_root_fdfsolver_root (s);					// current root value
		solve_status = gsl_root_test_delta (fermi_level, fermi_opt->in_guess(), 
											fermi_opt->abs_err(), fermi_opt->rel_err() );
		fermi_opt->set_init_guess (fermi_level);						

	}
	while (solve_status == GSL_CONTINUE && (fermi_opt->iter_out() < fermi_opt->max_iter_out()) );
	gsl_root_fdfsolver_free (s);			// free memory
	return fermi_level;
}

// дополнительно
double Fermi_searching::debye_length_calc(void) // Debye length calculation
{
	if (fermi_level == 0)
	{
		return 0;
	}
	else
	{
		return dim.charact_l() / 
			( sqrt(Fermi_searching::electro_neutral_eq_deriv(fermi_level, this)) );
	}
}


