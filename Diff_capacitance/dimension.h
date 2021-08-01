// Обезразмеривание. Возможны разные виды обезразмеривания в зависимости от хар длины.
 


#pragma once
#include "StdAfx.h"
#include "phys_const.h" 

using namespace std;
using namespace phys_const;



// dimensionless variables utils

class Dimension
{
private:
	double coordinate;		// coordinate in crystal (1d) 
	double potential;		// potential in crystal
	double fermi_level;		// Fermi level
	double band_gap_width;	// band gap width
	double n_t;				// impurities concentration 
	double capacity;		// capacity

	double coordinate_dless, potential_dless, fermi_level_dless, 
		band_gap_width_dless, n_t_dless, capacity_dless;	// dimensionless variables

	double charact_length;	// characteristic length
	double n_0;				// effective density of states coefficient
	
	double n_c, n_v;				// effective density of states
	double n_c_dless, n_v_dless;	// dimensionless variables



public:
	// default constructor
	Dimension():coordinate(0), potential(0), fermi_level(0), band_gap_width(0), capacity(0),
		coordinate_dless(0), potential_dless(0), fermi_level_dless(0), band_gap_width_dless(0),
		charact_length(0),  n_0(0), n_c(0), n_v(0), n_t(0), n_c_dless(0), n_v_dless(0), n_t_dless(0),
		capacity_dless(0) {}

	// set main variables
	Dimension& set_coord (double coord);				
	Dimension& set_pot (double pot);							
	Dimension& set_ferm_lev (double ferm_lev);				
	Dimension& set_band_gap (double band_gap);
	Dimension& set_n_t (double n_t_t, double permittivity);
	Dimension& set_capacity (double capacity_y, double permittivity);
	Dimension& set_coord_dless (double coord_dless);			
	Dimension& set_pot_dless (double pot_dless);				
	Dimension& set_ferm_lev_dless (double ferm_lev_dless);	
	Dimension& set_band_gap_dless (double band_gap_dless);
	Dimension& set_n_t_dless (double n_t_dless_s, double permittivity);
	Dimension& set_capacity_dless (double capacity_dless_s, double permittivity);
	
	// set characteristic length and n_0  
	Dimension& set_charact_l_n_0(double charact_l);

	// set characteristic length equal to termal length
	Dimension& set_charact_l_as_wave_l_n_0(void);

	Dimension& set_n_с (double emass, double permittivity);	//  n_c_dless  получается делением на n_0. если n_0 == 0 , то она полагается равной нулю.					
	Dimension& set_n_v (double hmass, double permittivity);	//  n_v_dless т получается делением на n_0. если n_0 == 0 , то она полагается равной нулю.	

	// ev to joule and vice versa 
	double ev_to_joule (double);
	double joule_to_ev (double);

	// output functions
	double coord (void) const {return coordinate;} 
	double pot (void) const {return potential;} 
	double ferm_lev (void) const {return fermi_level;}
	double band_gap (void) const {return band_gap_width;}
	double n_t_out (void) const {return n_t;}
	double capacity_out (void) const {return capacity;}
	double coord_dless (void) const {return coordinate_dless;}
	double pot_dless (void) const {return potential_dless;} 
	double ferm_lev_dless (void) const {return fermi_level_dless;}
	double band_gap_dless (void) const {return band_gap_width_dless;}
	double n_t_dless_out (void) const {return n_t_dless;}
	double capacity_dless_out (void) const {return capacity_dless;}
	double charact_l (void) const {return charact_length;}
	double n_0_out (void) const {return n_0;}
	double n_c_out (void) const {return n_c;}
	double n_v_out (void) const {return n_v;}
	double n_c_dless_out (void) const {return n_c_dless;}
	double n_v_dless_out (void) const {return n_v_dless;}
};



/////////////////////////////
// Dimension class test

class Dimension_test
{
private:
	Dimension test_object;
	int choice;
public:
	Dimension_test():choice(0) {}
	void print_menu (void);
	void set_choice(void);
	void run (void);
	void print_par(Dimension&);
};