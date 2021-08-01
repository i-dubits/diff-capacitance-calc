#include "StdAfx.h"
#include "phys_const.h"
#include "material_parameters.h"

#include <iostream>

using namespace phys_const;

// constructor
CMT::CMT(double xx, double th, double imp_c)
{
	vector<double> par; // param vector
	par.push_back(xx);
	x = xx;
	set_gap(par);
	set_th(th);
	set_imp_c(imp_c);
	set_emass(par);
	set_hmass(par);
	set_perm(par);
	
}

// set_x defines x and hence all other parameters apart from thickness and impurity_conc. 
material_parameters& CMT::set_x (double xx)
{
	vector<double> par;	// param vector
	par.push_back(xx);
	x = xx;
	set_gap(par);
	set_emass(par);
	set_hmass(par);
	set_perm(par);
	return *this;
}




	// Функции, задающие параметры КРТ
// эффективные массы, диэл проницаемость и ширина запрещенной зависят только от 
// стехиометрического состава КРТ (x)
material_parameters& CMT::set_th(double th)
{
	 thickness = th;
	 return *this;
}

material_parameters& CMT::set_imp_c(double imp_c)
{
	 impurity_conc = imp_c;
	 return *this;
}

	// Пока параметр только один - x.
// band_gap_width записана  в ЭВах
material_parameters& CMT::set_gap (vector<double> par)
{
	band_gap_width = -0.302 + 1.93 * par[0] + 5.35E-4*(1 - 2 * par[0])*
		temperature - 0.81*par[0]*par[0] + 0.832*par[0]*par[0]*par[0];
	return *this;
}

// effective_mass_el записана в системе СИ
material_parameters& CMT::set_emass(vector<double> par)
{
	effective_mass_el = free_electron_mass * 1/
		(1 + (( 18 - 3 * par[0] )*( band_gap_width + 0.8 - 0.26 * par[0] ))/
		( band_gap_width * ( band_gap_width + 1.2 - 0.39 * par[0] ) ) );
	return *this;
}

// effective_mass_hole записана в системе СИ
// для КРТ effective_mass_hole == effective_mass_el
material_parameters& CMT::set_hmass(vector<double> par)
{
	effective_mass_hole = free_electron_mass * 1/
		(1 + (( 18 - 3 * par[0] )*( band_gap_width + 0.8 - 0.26 * par[0] ))/
		( band_gap_width * ( band_gap_width + 1.2 - 0.39 * par[0] )) );
	return *this;
}

// permittivity по своей сути безразмерна 
material_parameters& CMT::set_perm (vector<double> par)
{
	permittivity = 20.5 - 15.5 * par[0] + 5.72 * par[0]*par[0];
	return *this;
}


// Германий

	// конструктор

Germanium::Germanium(double th, double emass, double hmass, double perm, double gap, double imp_c)
{
	vector<double> temp_vector[4]; // создан для преобразования double в вектор
	temp_vector[0].push_back(emass);
	temp_vector[1].push_back(hmass);
	temp_vector[2].push_back(perm);
	temp_vector[3].push_back(gap);
	

	set_th(th);
	set_imp_c(imp_c);
	set_emass(temp_vector[0]);
	set_hmass(temp_vector[1]);
	set_perm(temp_vector[2]);
	set_gap(temp_vector[3]);
}

	// функции, устанавливающие параметры (для германия они просто задаются руками, поэтому
	// функции триивиальны)

material_parameters& Germanium::set_th (double th)
{
	thickness = th;
	return *this;
}

material_parameters& Germanium::set_imp_c(double imp_c)
{
	 impurity_conc = imp_c;
	 return *this;
}

material_parameters& Germanium::set_emass (vector<double> par)
{
	effective_mass_el = par[0];
	return *this;
}

material_parameters& Germanium::set_hmass (vector<double> par)
{
	effective_mass_hole = par[0];
	return *this;
}

material_parameters& Germanium::set_perm (vector<double> par)
{
	permittivity = par[0];
	return *this;
}

material_parameters& Germanium::set_gap (vector<double> par)
{
	band_gap_width = par[0];
	return *this;
}

//////////////////////////////////////////////
//			Тестирование 
//	тестирование класса CMT
void CMT_test::print_menu(void)
{
	printf ("\n1.Default construct."
			"\n2.Construct. (test ONLY contructor, parameters cannot be changed and ""Print parameters"" does not work)"
			"\n3.Set thikness."
			"\n4.Set x."
			"\n5.Print parameters."
			"\n6.Set impurity level."
			"\n0.Exit.\n");
}

int CMT_test::set_choice (void)
{
	scanf ("%i", &choice);
	return choice;
}

void CMT_test::run (void)
{
	switch (choice)
	{

		case 1:
			{
				print_par(test_object);
			}
			break;
		case 2:
			{
				printf ("\nSet par: (x, thikness, impurity concentration): ");
				scanf ("%lf", &x_test);
				scanf ("%lf", &thikness_test);
				scanf ("%lf", &imp_c_test);
				CMT test_object (x_test, thikness_test, imp_c_test);
				print_par(test_object);
			}
			break;
		case 3:
			{
				printf ("\nSet thickness: ");
				scanf("%lf", &thikness_test);
				test_object.set_th(thikness_test);
			}
			break;
		case 4:
			{
				printf ("\nSet x: ");
				scanf("%lf", &x_test);	
				test_object.set_x(x_test);
			}
			break;
		case 5:
			{
				print_par(test_object);
			}
			break;
		case 6:
			{
				printf ("\nSet impurity level: ");
				scanf("%lf", &imp_c_test);
				test_object.set_imp_c(imp_c_test);
			}
			break;
		case 0:
			break;
		default:
			break;
	}
}
void  CMT_test::print_par (CMT& current)
{
	printf ("\nCMT parameters: \n"
						"x = %lf\n"
						"thickness = %lf\n"
						"impurity level = %lf\n"
						"effective_mass_el = %le\n"
						"effective_mass_hole = %le\n"
						"permittivity = %lf\n"
						"band_gap_width = %lf\n", current.x_par(),
												 current.th(),
												 current.imp_c(),
												 current.emass(),
												 current.hmass(),
												 current.perm(),
												 current.gap() );
}

// конец тестирования класса CMT