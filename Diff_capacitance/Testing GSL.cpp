// GSL tests

#include "stdafx.h"
#include "math.h"
#include "Testing GSL.h"

#include <iostream>

// integration
double func_for_QAG_adapt_int1(double x, void* params)
{
	double alpha = *(double* )params;
	return ( sqrt(x) * sqrt (1 + 2 * x) * (1 + 3 * x) * exp(x - alpha) )/
		( pow( (exp(x - alpha) + 1), 2) );
}

double func_for_QAG_adapt_int2(double x, void* params)
{
	double alpha = *(double* )params;
	return ( sqrt(x) * sqrt (1 + 2 * x) * (1 + 3 * x) )/
		( 4 * cosh((x - alpha)/2) * cosh((x - alpha)/2) ) ;

}

double func_for_QAQIU_adapt_int1(double x, void* params)
{
	double alpha = *(double* )params;
	return ( sqrt(x) * sqrt (1 + 2 * x) * (1 + 3 * x) )/
		( 4 * cosh((x - alpha)/2) * cosh((x - alpha)/2) ) ;

}

