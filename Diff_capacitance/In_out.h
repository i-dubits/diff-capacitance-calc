// input output routines

#pragma once
#include "stdafx.h"
#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"

#include "phys_const.h"
#include "material_parameters.h" 
#include "dimension.h"
#include "Concentration_functions.h"

class In_out
{
private:
	double v_s_max;
	double v_s_min;
	vector<double> thickness_vector;	// thickness vector 
	int point_number;	// point number exclude ends of the segment 
	vector<double> v_s_vector;			// input vector
	// vector<double> capacity_vector;		// output vector
public:
	// we should consider special case of v_s = 0
	vector<double> v_s_vector_initialization();

	// initializations
	void set_v_s_max(double v_s_max_x) {v_s_max = v_s_max_x;}
	void set_v_s_min(double v_s_min_n) {v_s_min = v_s_min_n;}
	void set_point_number(int point_number_r) {point_number = point_number_r;}
	void set_thickness_vector(vector<double> thikness){thickness_vector = thikness;}

	// output
	double v_s_max_out(void) const {return v_s_max;} 
	double v_s_min_out(void) const {return v_s_min;}
	int point_number_out(void) const {return point_number;} 
	vector<double> thickness_vector_out(void) const {return thickness_vector;}
	vector<double> v_s_vector_out(void) const {return v_s_vector;}
	// constructors
	In_out ():v_s_max(0), v_s_min(0), point_number(0) {}
	In_out (double v_s_min_n, double v_s_max_x, int point_number_r, vector<double> thickness_vector_r);
};