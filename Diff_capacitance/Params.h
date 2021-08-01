// connection between computational engine and GUI
// it receives problem parameters from GUI 

#pragma once
#include "stdafx.h"
#include <string>


using namespace std;

class Params
{

public:
	// свойства материала
	double imp_conc;			// impurity concentration (n_d - n_a)
	double eff_mass_el;			// the ratio of the effective mass of an electron to the mass of a free electron 
	double eff_mass_hole;		// the ratio of the effective mass of a hole to the mass of a free electron 
	double permittivity;		 
	double band_gap;			
	double thikness;			// thickness in meters
	double x;					// stechiometry
	
	// solver config
	double v_s_min;				// minimal surface potential (V) минимальный поверхностный потенциал (вольты)
	double v_s_max;				// maximal surface potential (V) 
	int point_number;			// number of points

	string output_file_path;	// path to output file

	// named constants, each of which corresponds to a different law of dispersion
	enum Dispersion
	{
		kein_fermi,				// Kane + Fermi-Dirac 
								// no approximations 
		par_fermi_approx,		// Parabolic + Fermi-Dirac 
								// approximations included
		par_fermi,				// Parabolic + Fermi-Dirac 
								// no approximations 
		par_boltzman			// Parabolic + Boltzmann
								// approximations are not needed
	};

	Dispersion dispersion;

	// constructors
	Params():
		imp_conc(1E22), 
		eff_mass_el(0.553), 
		eff_mass_hole(0.35), 
		permittivity(16.2),
		band_gap(0.661), 
		thikness(2.67E-8), 
		x(0), 
		output_file_path("results.txt"), 
		v_s_min(-0.6), 
		v_s_max(0.2), 
		point_number(100), 
		dispersion(par_fermi_approx) 
		{}

	Params(double imp_conc_c, double eff_mass_el_l, double eff_mass_hole_e, double permittivity_y, 
		double band_gap_p, double thikness_s, double v_s_min_n, double v_s_max_x, int point_number_r,
		string output_file_path_h , Dispersion dispersion_n , double x_x = 0);

	// methods
	void edit (Params* par);

};