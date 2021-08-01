// material parameters

// depending on the type of material, parameters can be calculated or set empirically 

#pragma once
#include "StdAfx.h"
#include <vector>

using namespace std;

class material_parameters
{
protected:
	double thickness;			// film thickness 
	double effective_mass_el;	// electron effective mass (SI units) 
	double effective_mass_hole; // hole effective mass (SI units) 
	double permittivity;		// permitivity 
	double band_gap_width;		// band gap width (in eV) 
	double impurity_conc;		// impurity conentration 
//	string dispersion_law;		// закон дисперсии

public:
	// method define protected variables
	virtual material_parameters& set_th (double th) = 0;			
	virtual material_parameters& set_emass (vector<double>) = 0;	 
	virtual material_parameters& set_hmass (vector<double>) = 0;	
	virtual material_parameters& set_perm (vector<double>) = 0;		
	virtual material_parameters& set_gap (vector<double>) = 0;		
	virtual material_parameters& set_imp_c (double imp_c) = 0;		
	
	// ouput material parameters 
	double th() const		{ return thickness; }
	double emass() const	{ return effective_mass_el; }
	double hmass() const	{ return effective_mass_hole; }
	double perm() const		{ return permittivity; }
	double gap() const		{ return band_gap_width; }
	double imp_c() const	{ return impurity_conc; }

	// default constructor
	material_parameters()
	{
		thickness = 0; effective_mass_el = 0; effective_mass_hole = 0;
		permittivity = 0;  band_gap_width = 0; impurity_conc = 0;
	}
		 

};

class CMT : public material_parameters	// cadmium mercury telluride
{
private:
	double x;											//  set stechiometry (CdxHg1-xT)
	material_parameters& set_emass (vector<double>);	
	material_parameters& set_hmass (vector<double>);	
	material_parameters& set_perm (vector<double>);		
	material_parameters& set_gap (vector<double>);		

public:
	material_parameters& set_th (double th);			
	material_parameters& set_imp_c (double imp_c);		
	material_parameters& set_x (double xx);				// set params based on stechiometry 	

	double x_par() const {return x;}

	// default constructor
	 CMT():x(0) { }

	 // constructor. x determines all parameters apart from thickness 
	 CMT(double xx, double th, double imp_c);
	

};

class Germanium:public material_parameters  // Germanium paramters

{
public:
	 material_parameters& set_th (double th);			
	 material_parameters& set_imp_c (double imp_c);		
	 material_parameters& set_emass (vector<double>);	
	 material_parameters& set_hmass (vector<double>);	
	 material_parameters& set_perm (vector<double>);	
	 material_parameters& set_gap (vector<double>);		

	 // default constructor
	 Germanium() {}
	 
	 // constructor
	  Germanium(double th, double emass, double hmass, double perm, double gap, double imp_c);

};


//////////////////////////////////////////////
// Tests 

class CMT_test
{
private:
	int choice;
	double x_test, thikness_test, imp_c_test;
	CMT test_object;
public:
	CMT_test()
	{ choice = 0; x_test = 0; thikness_test = 0; }

	void print_menu (void);
	int set_choice (void);
	void run (void);
	void print_par (CMT&);
};
