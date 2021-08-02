// connection between computational engine and GUI
// it receives problem parameters from GUI 

#include "stdafx.h"
#include "Params.h"

// constructors

Params::Params(double imp_conc_c, double eff_mass_el_l, double eff_mass_hole_e, double permittivity_y, 
		double band_gap_p, double thikness_s, double v_s_min_n, double v_s_max_x, int point_number_r, 
		string output_file_path_h, Dispersion dispersion_n ,double x_x )
{
	imp_conc = imp_conc_c;
	eff_mass_el = eff_mass_el_l;
	eff_mass_hole = eff_mass_hole_e;
	permittivity = permittivity_y;
	band_gap = band_gap_p;
	thikness = thikness_s;
	x = x_x;
	v_s_min = v_s_min_n;
	v_s_max = v_s_max_x;
	point_number = point_number_r;
	output_file_path = output_file_path_h;
	dispersion = dispersion_n;	
}
