// functions which compute concentrations of holes and electrons 
// parabolic + Boltzmann, parabolic + Fermi-Dirac, relativistic  + Fermi-Dirac

#pragma once
#include "StdAfx.h"
#define _USE_MATH_DEFINES
#include "math.h"

// we suggest that Concentration_functions are the same for holes and electrons
class Concentration_functions
{
public:
	virtual double concentration (double x, void* param) = 0;		// concentration calculation
	virtual double concentration_deriv (double x, void* param) = 0;	// concentration derivative
	virtual void concentration_fdf (double x, void* param,			// this functions is needed by GSL
										 double* y, double *dy) = 0;
};


// Kane + Fermi-Dirac 
// asymptotics are used during calculations
class Kein_fermi_asympt_conc: public Concentration_functions
{
public:
	double concentration (double x, void* param);			
	double concentration_deriv (double x, void* param);		
	void concentration_fdf (double x, void* param,	
										 double* y, double *dy);

};

// Kane + Fermi-Dirac 
// approximations are not applied
class Kein_fermi_conc: public Concentration_functions
{
public:
	double concentration (double x, void* param);			
	double concentration_deriv (double x, void* param);		
	void concentration_fdf (double x, void* param,	
										 double* y, double *dy);

};



// parabolic + Fermi-Dirac 
// approximation is used during calculations
class Par_fermi_approx_conc: public Concentration_functions
{
public:
	double concentration (double x, void* param);																				
	double concentration_deriv (double x, void* param);		
	void concentration_fdf (double x, void* param, 
										double* y, double *dy);	
};


// parabolic + Boltzmann
// approximations and asymptotics are not needed
class Par_boltzman_conc: public Concentration_functions
{
public:
	double concentration (double x, void* param);																				
	double concentration_deriv (double x, void* param);		
	void concentration_fdf (double x, void* param, 
										double* y, double *dy);	
};

// parabolic + Fermi-Dirac
// approximations are not applied
class Par_fermi_conc: public Concentration_functions
{
private:
	static double integrand_for_conc (double x, void* param);
	static double integrand_for_conc_deriv (double x, void* param);
public:
	double concentration (double x, void* param);																				
	double concentration_deriv (double x, void* param);		
	void concentration_fdf (double x, void* param, 
										double* y, double *dy);	
};

// approximations of Fermi-Dirac integral and integrand for Kane's dispersion 
class Concentrations_utilities
{
private:
	Concentrations_utilities() { }; // class has no constructor

public:
	// k - order of Fermi-Dirac integral
	static double fermi_dirac_approx (double x, double k); 
	// coefficients a, b, с (used in approximation)
	static double a_coeff (double k); 
	static double b_coeff (double k);
	static double с_coeff (double k);

	// подынт выражения для Кейна
	static double integrand_for_conc_Kein_fermi (double x, void* param);	// concentration integrand 
	static double integrand_for_conc_deriv_Kein_fermi (double x, void* param);	// concentration derivative integrand 
};




