// input output routines

#include "stdafx.h"
#include "in_out.h"

vector<double> In_out::v_s_vector_initialization()
{
	double subinterval;
	double current;

	current = v_s_min;
	subinterval = ( v_s_max - v_s_min ) / ( point_number + 1);

	// we take into account the ends of the segment 
	for (int i = 0; i < point_number + 2; i++)
	{
		if (current == 0) // integral diverges if v_s == 0 
		{
			v_s_vector.push_back(current + 1e-3*subinterval);
			current += subinterval;
		}
		else
		{
			v_s_vector.push_back(current);
			current += subinterval;
		}
	}

	return v_s_vector;
}

// constructor

In_out::In_out (double v_s_min_n, double v_s_max_x, int point_number_r, vector<double> thickness_vector_r)
{
	v_s_min = v_s_min_n;
	v_s_max = v_s_max_x;
	point_number = point_number_r;
	thickness_vector = thickness_vector_r;
}