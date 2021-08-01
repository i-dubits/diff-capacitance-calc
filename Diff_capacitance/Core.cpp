// interaction with GUI
#include "stdafx.h"
#include "../GUI/MainDlg.h"

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

//#include <fstream>
#include <ostream>
#include <sstream>
#include <iomanip>

void Core (Params* params, MainDlg *dlg)
{

	Dimension dim_current;
	vector<double> thickness_vector_r;		// thickness vector 
	vector<double> v_s_vector;				// surface potential vector 	
	vector<double> capacity_vector;			// capacity vector 

	double ferm_level_current = 0;	// Fermi level уровень Ферми
	double character_length = 0;	// characteristic length

#ifdef _DEBUG
	assert(dlg && "wrong main dialog");
#endif // _DEBUG

	dlg->WriteLine("Surf. Potential, V    Capacitance, F/m2");


	character_length = params->thikness*2;	


	//character_length = params->thikness/2;


	thickness_vector_r.push_back(params->thikness / character_length);
	


	// set dispersion relation 
	switch (params->dispersion)
	{
	case params->kein_fermi:
		{
			Kein_fermi_conc current_conc_func;
			// material parameters

			Germanium curr_material(1, params->eff_mass_el * free_electron_mass,
					params->eff_mass_hole * free_electron_mass,
					params->permittivity, params->band_gap, params->imp_conc);	

			// Fermi level calculation		
			Fermi_searching_options f_opt(0, 1e-6, 10000, 100, 0, 1e-6);	// options
			Fermi_searching f_search (&current_conc_func, &f_opt, &curr_material);
			f_search.dim_config(character_length);	//  characteristic length
			ferm_level_current = f_search.newton_method();
			dim_current.set_ferm_lev_dless(ferm_level_current);	
			
			// capacity calculation
			Capacity_calculation cap_calc (-4, 1e-8, 0, 30000, ferm_level_current,
									&current_conc_func, &curr_material, &f_search, dlg, 1e-7);	
			cap_calc.dim_config(character_length);	//  characteristic length

			// dim_current class options
			dim_current.set_charact_l_n_0(character_length); //  characteristic length
			dim_current.set_band_gap( dim_current.ev_to_joule( curr_material.gap() ) ); 
			// mat_par -> impurity_conc и dim -> n_t это одна и таже величина!
			dim_current.set_n_t(  curr_material.imp_c(), curr_material.perm() );	// set concentration
			dim_current.set_n_с ( curr_material.emass(), curr_material.perm() );	
			dim_current.set_n_v ( curr_material.hmass(), curr_material.perm() );	

			//dolver
			// just for testing of the progres control:
			/*dlg->setProgress(15);*/
			In_out in_out_current (dim_current.set_pot(params->v_s_min).pot_dless(), 
				dim_current.set_pot(params->v_s_max).pot_dless(), params->point_number, thickness_vector_r);
			v_s_vector = in_out_current.v_s_vector_initialization();
				// thickness vector has only one component
			curr_material.set_th(in_out_current.thickness_vector_out()[0]);
			capacity_vector = cap_calc.capacity_calculation(v_s_vector);
			
			for (size_t k = 0; k < v_s_vector.size(); k ++)
			{
				// output in dimension units
			
				dim_current.set_pot_dless(v_s_vector[k]);
				dim_current.set_capacity_dless(capacity_vector[k], curr_material.perm());
						
				ostringstream to;
				to << right << setw(15) << dim_current.pot() 
					<< std::string(6, ' ') 
					<< right << setw(15) << dim_current.capacity_out()/* << endl*/;
				dlg->WriteLine(to.str().c_str());
			}
			
		}
		break;

	case  params->par_fermi_approx:
		{
			Par_fermi_approx_conc current_conc_func;
			// material parameters

			Germanium curr_material(1, params->eff_mass_el * free_electron_mass,
					params->eff_mass_hole * free_electron_mass,
					params->permittivity, params->band_gap, params->imp_conc);	

			// Fermi level	
			Fermi_searching_options f_opt(0, 1e-6, 10000, 100, 0, 1e-6);	
			Fermi_searching f_search (&current_conc_func, &f_opt, &curr_material);
			f_search.dim_config(character_length);	
			ferm_level_current = f_search.newton_method();
			dim_current.set_ferm_lev_dless(ferm_level_current);

			// capacity
			Capacity_calculation cap_calc (-4, 1e-8, 0, 30000, ferm_level_current,
									&current_conc_func, &curr_material, &f_search, dlg, 1e-7);	
			cap_calc.dim_config(character_length);	

			// dim_current class options
			dim_current.set_charact_l_n_0(character_length); 
			dim_current.set_band_gap( dim_current.ev_to_joule( curr_material.gap() ) ); 
			dim_current.set_n_t(  curr_material.imp_c(), curr_material.perm() );	
			dim_current.set_n_с ( curr_material.emass(), curr_material.perm() );	
			dim_current.set_n_v ( curr_material.hmass(), curr_material.perm() );	

			//solver
			In_out in_out_current (dim_current.set_pot(params->v_s_min).pot_dless(), 
				dim_current.set_pot(params->v_s_max).pot_dless(), params->point_number, thickness_vector_r);
			v_s_vector = in_out_current.v_s_vector_initialization();
			curr_material.set_th(in_out_current.thickness_vector_out()[0]);
			capacity_vector = cap_calc.capacity_calculation(v_s_vector);
			
			//output to file 

					for (size_t k = 0; k < v_s_vector.size(); k ++)
					{


						dim_current.set_pot_dless(v_s_vector[k]);
						dim_current.set_capacity_dless(capacity_vector[k], curr_material.perm());
						
						ostringstream to;

						to	<< right << setw(15) << dim_current.pot() 
							<< std::string(6, ' ') 
							<< right << setw(15) << dim_current.capacity_out()/* << endl*/;
						dlg->WriteLine(to.str().c_str());

					}

		}
		break;

	case params->par_fermi:
		{
			Par_fermi_conc current_conc_func;

			Germanium curr_material(1, params->eff_mass_el * free_electron_mass,
					params->eff_mass_hole * free_electron_mass,
					params->permittivity, params->band_gap, params->imp_conc);	

			Fermi_searching_options f_opt(0, 1e-6, 10000, 100, 0, 1e-6);	
			Fermi_searching f_search (&current_conc_func, &f_opt, &curr_material);
			f_search.dim_config(character_length);	
			ferm_level_current = f_search.newton_method();
			dim_current.set_ferm_lev_dless(ferm_level_current);

			Capacity_calculation cap_calc (-4, 1e-8, 0, 30000, ferm_level_current,
									&current_conc_func, &curr_material, &f_search, dlg, 1e-7);	
			cap_calc.dim_config(character_length);	


			dim_current.set_charact_l_n_0(character_length); 
			dim_current.set_band_gap( dim_current.ev_to_joule( curr_material.gap() ) ); 

			dim_current.set_n_t(  curr_material.imp_c(), curr_material.perm() );	
			dim_current.set_n_с ( curr_material.emass(), curr_material.perm() );	
			dim_current.set_n_v ( curr_material.hmass(), curr_material.perm() );	

			In_out in_out_current (dim_current.set_pot(params->v_s_min).pot_dless(), 
				dim_current.set_pot(params->v_s_max).pot_dless(), params->point_number, thickness_vector_r);
			v_s_vector = in_out_current.v_s_vector_initialization();
			curr_material.set_th(in_out_current.thickness_vector_out()[0]);
			capacity_vector = cap_calc.capacity_calculation(v_s_vector);
			
			//ofstream to (params->output_file_path,  ios_base::app|ios_base::out);
					for (size_t k = 0; k < v_s_vector.size(); k ++)
					{

						dim_current.set_pot_dless(v_s_vector[k]);
						dim_current.set_capacity_dless(capacity_vector[k], curr_material.perm());
						
						ostringstream to;
						to << right << setw(15) << dim_current.pot() 
							<< std::string(6, ' ') 
							<< right << setw(15) << dim_current.capacity_out()/* << endl*/;
						dlg->WriteLine(to.str().c_str());
					}
		}
		break;

	case params->par_boltzman:
		{
			Par_boltzman_conc current_conc_func;



			Germanium curr_material(1, params->eff_mass_el * free_electron_mass,
					params->eff_mass_hole * free_electron_mass,
					params->permittivity, params->band_gap, params->imp_conc);	

		
			Fermi_searching_options f_opt(0, 1e-6, 10000, 100, 0, 1e-6);	
			Fermi_searching f_search (&current_conc_func, &f_opt, &curr_material);
			f_search.dim_config(character_length);	
			ferm_level_current = f_search.newton_method();
			dim_current.set_ferm_lev_dless(ferm_level_current);	

			
			Capacity_calculation cap_calc (-4, 1e-8, 0, 30000, ferm_level_current,
									&current_conc_func, &curr_material, &f_search, dlg, 1e-7);	
			cap_calc.dim_config(character_length);	

			dim_current.set_charact_l_n_0(character_length); 
			dim_current.set_band_gap( dim_current.ev_to_joule( curr_material.gap() ) ); 
			// mat_par -> impurity_conc и dim -> n_t это одна и таже величина!
			dim_current.set_n_t(  curr_material.imp_c(), curr_material.perm() );	
			dim_current.set_n_с ( curr_material.emass(), curr_material.perm() );	
			dim_current.set_n_v ( curr_material.hmass(), curr_material.perm() );	

			
			In_out in_out_current (dim_current.set_pot(params->v_s_min).pot_dless(), 
				dim_current.set_pot(params->v_s_max).pot_dless(), params->point_number, thickness_vector_r);
			v_s_vector = in_out_current.v_s_vector_initialization();
				
			curr_material.set_th(in_out_current.thickness_vector_out()[0]);
			capacity_vector = cap_calc.capacity_calculation(v_s_vector);
			
			//ofstream to (params->output_file_path,  ios_base::app|ios_base::out);
					for (size_t k = 0; k < v_s_vector.size(); k ++)
					{
					
						dim_current.set_pot_dless(v_s_vector[k]);
						dim_current.set_capacity_dless(capacity_vector[k], curr_material.perm());
						
						ostringstream to;
						to << right << setw(15) << dim_current.pot() 
							<< std::string(6, ' ') 
							<< right << setw(15) << dim_current.capacity_out()/* << endl*/;
						dlg->WriteLine(to.str().c_str());
					}
		}
		break;

	default:
		break;
	}

	
}
