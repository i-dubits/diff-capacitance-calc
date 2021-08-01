// Diff_capacitance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "material_parameters.h"
#include "phys_const.h" 
#include "dimension.h"
#include "Concentration_functions.h"
#include "Fermi_searching.h" 
#include "Capacity_calc.h"
#include "In_out.h"
#include "Testing GSL.h"
#include "Params.h"
#include "Core.h"
#include <gsl/gsl_integration.h>

#include <fstream>
#include <iostream>
#include <string>



using namespace std;

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

int _tmain(int argc, _TCHAR* argv[])
{
	CMT_test test_CMT;		// test CMT class
	Dimension_test test_D;	// test Dimension class
	int your_choice;  

	your_choice = 1;

	ios_base::sync_with_stdio();

	while(your_choice != 0)
	{
		printf ("\nIlya Dubitskiy, 2011\n\n\n\n Menu (type menu item (ex.: 1) )."
			"\n1.Capacity calculation (semiconductor with parabolic dispersion law )."
			"\n2.About."
			"\n3.Testing fermi level searching func."
			"\n4.Testing Core and Params."
			//"\n12.Capacity calculation CMT."
			"\n0.Exit.\n");

		//cin >> your_choice;
		scanf("%i", &your_choice);
		switch(your_choice)
		{

		case 4:
			{
				string output_file_path = "D:\\test_core.txt";
				Params::Dispersion disp = Params::par_boltzman;
				Params par(1E22, 0.553, 0.35, 16.2, 0.661, 2.67E-8, -0.6, 0.2, 200, output_file_path, disp, 0);
				Core(&par, nullptr);

			}
			break;
		case 0:
			break;
		default:
			break;
		}
	}
	return 0;
}

