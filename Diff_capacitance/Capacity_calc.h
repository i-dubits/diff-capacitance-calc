// Calculation of differential capacitance
// based on the paper "On the field effect in thin films of semiconductors with Kane’s charge-carrier dispersion relation"

#pragma once
#include "stdafx.h"
#include "../GUI/MainDlg.h"

#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"

#include "phys_const.h"
#include "material_parameters.h" 
#include "dimension.h"
#include "Concentration_functions.h"
#include "Fermi_searching.h" 


// #include "Fermi_searching.h"

// work with dimensionless quantities

class Capacity_calculation
{
private:
		// variables
	double fermi_level;	// Fermi level
	double abserr;		// absolute error of integral calculations
	double relerr;		// relative error of iteration procedure
	double v_s;			// v_s - the value of the potential on the front side of the semiconductor
						// the value of the potential on the back side of the semiconductor is set to zero
	int max_iter;		// maximal number of iterations
	int curr_iter;		// current iteration number

	double abserr_conv_test;	// absolute error of iteration procedure convergence test


	vector<double> v_l_deriv_squared_vector;	// result vector
	vector<double> v_s_deriv_vector;			// result vector

		// classes					
	Dimension dim;								// dimensions 
	Concentration_functions* conc_func;			// dispersion relations and approximations
	material_parameters* mat_par;				// material parameters
	
		// methods
	// right-hand side of Poisson equation  (it is written in the form compatible with GSL)
	static double second_member_pausson (double u, void * param);

	

	// ntegrand for h function
	static double integrate_for_h_func (double u, void* param);
	
	// test for convergence of a series of successive approximations 
	// analogue of gsl_root_test_delta  from root finding GSL
	// 0 - fault, 1 - success
	int convergence_test (double x_1, double x_0);

public:

	MainDlg *dlg;	// // MFC class, which is needed for GUI (in particular for spinMessageLoop())

	// temp class
	Fermi_searching* f_search_temp;


	double v_l_deriv_squared_auxiliary;	// auxillary variable for GSL 

	//  right-hand side of Poisson equation integration over potential

	double integrate_second_member_pausson(double u, void* param); 

	// h function (see the paper)
	double h_function (double v_l_deriv_squared);


	// iteration procedure (see the paper)
	// input v_s; output v_l_deriv_squared; argument - initial approximation
	double iteration_proc (double v_l_deriv_squared_initial);

	// function calculates v_s_deriv with known v_l_deriv_squared
	// signs of derivative and potentail are choosen according to the following equation
	// sgn(v_deriv) = -sgn(v_s) 
	double v_s_deriv_calc ( double v_l_deriv_squared);

	// surface charge density calculation
	double suface_charge_density_calc ( double v_s_deriv, double v_l_deriv_squared);

	// capacity calculation
		// input: vector v_s
		// output: vector ñ
	vector<double> capacity_calculation (vector<double> v_s_vector);

	// dim class initialization
	void dim_config (void);
	void dim_config (double length);

	// constructors
	Capacity_calculation():abserr(0), relerr(0), v_s(0),
		max_iter(0), curr_iter(0), fermi_level(0), abserr_conv_test(0), dlg(nullptr){}
	Capacity_calculation( double v_s, double abserr, double relerr, int max_iter,
						double fermi_level_l, Concentration_functions*, material_parameters*,Fermi_searching*, MainDlg *dlg, double abserr_conv_testt = -1); 
								// error of iteration procedure is set to -1 by defualt because of compatibility issues
	// variables initialization
	void set_v_s (double v_s_s) { v_s = v_s_s;}
	void set_abserr (double abserr_r) { abserr = abserr_r;}
	void set_relerr (double relerr_r) { relerr = relerr_r;}
	void set_max_iter (int max_iter_r) { max_iter = max_iter_r;}
	void set_ferm_lev (double fermi_level_l) { fermi_level = fermi_level_l;}
	
	// variables outputs
	double v_s_out (void) const {return v_s;}
	double abserr_out (void) const {return abserr;}
	double relerr_out (void) const {return relerr;}
	double max_iter_out (void) const {return max_iter;}
	double ferm_lev_out (void) const {return fermi_level;}
	int iter_out (void) const {return curr_iter;}

};