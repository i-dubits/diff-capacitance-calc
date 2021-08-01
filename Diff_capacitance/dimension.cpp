// dimensionless methods

#include "stdafx.h"
#include "dimension.h"
#include "phys_const.h"
#define _USE_MATH_DEFINES
#include "math.h"

#include <iostream>

#define _USE_MATH_DEFINES 

using namespace std;
using namespace phys_const;


// set characteristic length and n_0
Dimension& Dimension::set_charact_l_n_0(double charact_l)
{
	 charact_length = charact_l;
	 n_0 = ( electric_const * boltzmann * temperature )/ 
		 ( charact_length *  charact_length * el_charge * el_charge );
	 return *this;
}

// set characteristic length equal to term length 
Dimension&  Dimension::set_charact_l_as_wave_l_n_0(void)
{
	charact_length = thermal_wavelength;
	 n_0 = ( electric_const * boltzmann * temperature ) / 
		 ( charact_length *  charact_length * el_charge * el_charge );
	 return *this;
}


// set main quantities
Dimension& Dimension::set_coord(double coord)
{
	coordinate = coord;
	if (charact_length != 0)
	{
		coordinate_dless = coordinate / charact_length;
	}
	else
	{
		coordinate = 0; coordinate_dless = 0;
	}
	return *this;
}

Dimension& Dimension::set_pot(double pot)
{
	potential = pot;
	potential_dless = ( el_charge * potential ) / ( boltzmann * temperature );	
	return *this;
}

Dimension& Dimension::set_ferm_lev(double ferm_lev)
{
	fermi_level = ferm_lev;
	fermi_level_dless = fermi_level / ( boltzmann * temperature );
	return *this;
}

Dimension& Dimension::set_band_gap(double band_gap)
{
	band_gap_width = band_gap;
	band_gap_width_dless = band_gap_width /  ( boltzmann * temperature );
	return *this;
}
Dimension& Dimension::set_n_t (double n_t_t, double  permittivity)
{
	n_t = n_t_t;
	if (n_0 != 0)			// you cannot divide by zero 
	{
		n_t_dless = n_t / ( permittivity * n_0 );
	}
	else
	{
		n_t = 0; n_t_dless = 0; 
	}

	return *this; 
}

Dimension& Dimension::set_capacity (double capacity_y, double permittivity)
{
	capacity = capacity_y;
	if (permittivity != 0)
	{
		capacity_dless = charact_length / (permittivity * electric_const) * capacity;
	}
	else
	{
		capacity_dless = 0;
	}

	return *this;
}

Dimension& Dimension::set_n_t_dless (double n_t_dless_s, double permittivity)
{
	n_t_dless = n_t_dless_s;
	n_t = n_t_dless * permittivity * n_0 ;
	
	return *this; 	
}


Dimension& Dimension::set_coord_dless(double coord_dless)
{
	coordinate_dless = coord_dless;
	coordinate = coordinate_dless * charact_length;
	return *this;
}

Dimension& Dimension::set_pot_dless(double pot_dless)
{
	potential_dless = pot_dless;
	potential = ( boltzmann * temperature * potential_dless)/el_charge;
	return *this;
}

Dimension& Dimension::set_ferm_lev_dless(double ferm_lev_dless)
{
	 fermi_level_dless = ferm_lev_dless;
	 fermi_level = fermi_level_dless * ( boltzmann * temperature );
	 return *this;
}

Dimension& Dimension::set_band_gap_dless(double band_gap_dless)
{
	band_gap_width_dless = band_gap_dless;
	band_gap_width = band_gap_width_dless * ( boltzmann * temperature );
	return *this;
}

Dimension& Dimension::set_capacity_dless (double capacity_dless_s, double permittivity)
{
	capacity_dless = capacity_dless_s;
	if (charact_length != 0)
	{
		capacity =  (permittivity * electric_const) / (charact_length) * capacity_dless;
	}
	else
	{
		capacity = 0;
	}

	return *this;
}

// set effective density of states

Dimension& Dimension::set_n_с(double emass, double permittivity)
{
	n_c = 2 * pow ( ( emass * boltzmann * temperature ) / 
		( 2 * M_PI * h_bar* h_bar ), 1.5);
	if (n_0 != 0) // на ноль делить нельзя
	{
		n_c_dless = n_c / ( permittivity * n_0 );
	}
	else
	{
		n_c = 0; n_c_dless = 0; 
	}

	return *this;
}

Dimension& Dimension::set_n_v(double hmass, double permittivity)
{
	n_v = 2 * pow ( ( hmass * boltzmann * temperature ) / 
		( 2 * M_PI * h_bar* h_bar ), 1.5);
	if (n_0 != 0) // you cannot divide by zero 
	{
		n_v_dless = n_v / ( permittivity * n_0 );
	}
	else
	{
		n_v = 0; n_v_dless = 0; 
	}

	return *this;
}

// ev to joule and vice versa
double Dimension::ev_to_joule (double ev)
{
	return ev * 1.602176487e-19;
}

double Dimension::joule_to_ev (double joule)
{
	return 0.624150964e19 * joule;
}

/////////////////////////////
// test Dimension class

void Dimension_test::print_menu (void)
{
	printf ("\nFirst set characteristic length and n_0!"
			"\n1.Set coord."
			"\n2.Set potential."
			"\n3.Set fermi_level"
			"\n4.Set band_gap_width."
			"\n5.Set coord dimensionless."
			"\n6.Set potential dimensionless."
			"\n7.Set fermi_level dimensionless"
			"\n8.Set band_gap_width dimensionless."
			"\n9.Set charact_length and n_0."
			"\n10.Set charact_length as thermal wavelength and n_0."
			"\n11.Set n_c."
			"\n12.Set n_v."
			"\n13.Test ev/joule converter."
			"\n14.Print par."
			"\n15.Set n_t."
			"\n16.Set n_t dimensionless."
			"\n17.Set capacity."
			"\n18.Set capacity_dless."
			"\n0.Exit.\n");
}

void Dimension_test::set_choice(void)
{
	scanf ("%i", &choice);
}

void Dimension_test::print_par(Dimension& current)
{
	printf ("\nDimension variables: \n"
						"coord, coord_dless = %le, %le\n"
						"pot, pot_dless = %le, %le\n"
						"fermi, fermi_dless = %le, %le\n"
						"band_gap, band_gap_dless = %le, %le\n"
						"charact_length,  n_0 = %le, %le\n"
						"n_c,  n_c_dless = %le, %le\n"
						"n_v,  n_v_dless = %le, %le\n"
						"n_t,  n_t_dless = %le, %le\n"
						"capacity, capacity_dless = %le, %le\n",
						current.coord(), current.coord_dless(),
						current.pot(), current.pot_dless(),
						current.ferm_lev(), current.ferm_lev_dless(),
						current.band_gap(), current.band_gap_dless(),
						current.charact_l(), current.n_0_out(), 
						current.n_c_out(), current.n_c_dless_out(),
						current.n_v_out(), current.n_v_dless_out(),
						current.n_t_out(), current.n_t_dless_out(),
						current.capacity_out(), current.capacity_dless_out() );
}

void Dimension_test::run(void)
{
	double temp, temp_2;
	
	
	switch (choice)
	{
	case 1:
		{
			printf ("\nType coord (don't forget to set charact_length first): ");
			scanf ("%lf", &temp);
			test_object.set_coord(temp);
		}
		break;
	case 2:
		{
			printf ("\nType pot: ");
			scanf ("%lf", &temp);
			test_object.set_pot(temp);
		}
		break;
	case 3:
		{
			printf ("\nType fermi level: ");
			scanf ("%lf", &temp);
			test_object.set_ferm_lev(temp);
		}
		break;
	case 4:
		{
			printf ("\nType band_gap_width: ");
			scanf ("%lf", &temp);
			test_object.set_band_gap(temp);
		}
		break;
	case 5:
		{
			printf ("\nType coord dimensionless (don't forget to set charact_length first): ");
			scanf ("%lf", &temp);
			test_object.set_coord_dless(temp);
	}
	break;
	case 6:
		{
			printf ("\nType potential dimensionless: ");
			scanf ("%lf", &temp);
			test_object.set_pot_dless(temp);
		}
		break;
	case 7:
		{
			printf ("\nType fermi_level dimensionless: ");
			scanf ("%lf", &temp);
			test_object.set_ferm_lev_dless(temp);
		}
		break;
	case 8:
		{
			printf ("\nType band_gap_width dimensionless: ");
			scanf ("%lf", &temp);
			test_object.set_band_gap_dless(temp);
		}
		break;
	case 9:
			{
			printf ("\nType charact_length: ");
			scanf ("%lf", &temp);
			test_object.set_charact_l_n_0(temp);
		}
		break;
	case 10:
		{		
			test_object.set_charact_l_as_wave_l_n_0();
		}
		break;
	case 11:
		{
			printf ("\nType emass and permittivity( don't forget to set n_0 first): ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_n_с(temp, temp_2);
		}
		break;
	case 12:
		{
			printf ("\nType emass and permittivity( don't forget to set n_0 first): ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_n_v(temp, temp_2);
		}
		break;
	case 13:
		{
			printf ("\nType energy in ev: ");
			scanf ("%lf", &temp);
			printf("\n in joules: %le", test_object.ev_to_joule(temp) );
			printf ("\nRevers conversion: %le", test_object.joule_to_ev(test_object.ev_to_joule(temp)));
		}
		break;
	case 14:
		print_par(test_object);
		break;
	case 15:
		{
			printf ("\nType n_t and permittivity( don't forget to set n_0 first): ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_n_t(temp, temp_2);
		}
		break;
	case 16:
		{
			printf ("\nType n_t_dless and permittivity( don't forget to set n_0 first): ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_n_t_dless(temp, temp_2);
		}
		break;
	case 17:
		{
			printf ("\nType capacity and permittivity: ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_capacity(temp, temp_2);
		}
		break;
	case 18:
		{
			printf ("\nType capacity_dless and permittivity( don't forget to set char_length first): ");
			scanf ("%lf", &temp);
			scanf ("%lf", &temp_2);
			test_object.set_capacity_dless(temp, temp_2);
		}
		break;

	case 0:
		break;
	default:
		break;
	}
	
}

