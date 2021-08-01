// physical constants in SI

#pragma once
#include "StdAfx.h"
#define _USE_MATH_DEFINES
#include "math.h"

using namespace std;


namespace phys_const
{
	const double boltzmann =  1.3806504E-23;			
	const double temperature = 300;					
	const double free_electron_mass = 9.10938215E-31;	
	const double h_bar = 1.054571628E-34;				
	const double electric_const = 8.854187817620E-12;	
	const double el_charge = 1.602176487E-19;					
	
	const double thermal_wavelength = sqrt( (4 * M_PI * M_PI * h_bar* h_bar) / 
		(2 * free_electron_mass * boltzmann * temperature) );
												// thermal de Broglie wavelength

}