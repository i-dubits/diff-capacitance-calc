// Fermi level calculation
#pragma once
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"

#include "phys_const.h"
#include "material_parameters.h" 
#include "dimension.h"
#include "Concentration_functions.h"

class Fermi_searching_options	// settings
{
private:
	double absolute_err;		// absolute error
	double relative_err;		// relative error
	long iter;			// current step number 
	long max_iter;			// maximal number of steps
	double initial_guess;		// initial_guess
	
	// integration routines
	double int_abs_err;		// absolute integration error
	double int_rel_err;		// relative integration error
public:
	int iter_status;			

	double variable_for_conc_fun;		// integrand parameter  
	double variable_for_conc_fun_deriv;	// integrand derivative parameter 
	double* result_int_abs_err;		// measured absolute error


	Fermi_searching_options& set_abs_err (double abs_err) {absolute_err = abs_err; return *this;}			// set abs error
	Fermi_searching_options& set_rel_err (double rel_err) {relative_err = rel_err; return *this;}			// set rel error
	Fermi_searching_options& set_max_iter (long max_iter_r) {max_iter = max_iter_r; return *this;}			// set maimal number of steps
	Fermi_searching_options& set_init_guess (double in_guess) {initial_guess = in_guess; return *this;}		// set initial guess
	// integration routines
	Fermi_searching_options& set_int_abs_err (double int_abs_err_r) {int_abs_err = int_abs_err_r; return *this;}	// set abs integration error
	Fermi_searching_options& set_int_rel_err (double int_rel_err_r) {int_rel_err = int_rel_err_r; return *this;}	// set rel integration error
	
	void add_one_iterate(void) {iter += 1;} // increase step number by one

	double abs_err() const {return absolute_err;}		
	double rel_err() const {return relative_err;}		
	long iter_out() const {return iter;}				
	long max_iter_out() const {return max_iter;}		
	int iter_status_out() const {return iter_status;}	
	double in_guess() const {return initial_guess;}		
	// integration routines
	double int_abs_err_out() const {return int_abs_err;}	
	double int_rel_err_out() const {return int_rel_err;}	

	
	// constructors
	Fermi_searching_options():absolute_err(0),  relative_err(0), iter(0), iter_status(0), initial_guess(0){}
	Fermi_searching_options(double abs_err, double rel_err, long max_iterr, double in_guess, double int_abs_err_r, double int_rel_err_r);
};

class Fermi_searching
{
private:
	double fermi_level;							
	Dimension dim;								
	Concentration_functions* conc_func;			
	material_parameters* mat_par;				
	Fermi_searching_options *fermi_opt;			
	


public:
	static double electro_neutral_eq ( double x, void* param );		// electroneutrality equation
	static double electro_neutral_eq_deriv( double x, void* param );		// its derivative
	static void electro_neutral_eq_fdf( double x, void* param, double* y, double *dy);	// function for GSL


	//  constructors
	Fermi_searching():fermi_level(0) {}  
	Fermi_searching( Concentration_functions*, Fermi_searching_options*,  material_parameters*);	
	void dim_config (void);
	
	// set length for dimensionless operations
	void dim_config (double length);


	double fermi_level_out () const {return fermi_level;}
	

	Fermi_searching_options* fermi_s_opt_out () const {return fermi_opt;}



	Dimension dim_out () const {return dim;}	

	// methods
	double newton_method();	// return dimensionless Fermi level
	double debye_length_calc(void); 
};
