// GSL tests

#pragma once
#include "StdAfx.h"
#define _USE_MATH_DEFINES
#include "math.h"

// integration
double func_for_QAG_adapt_int1(double x, void* params);		// regular adaptive integration 1
double func_for_QAG_adapt_int2(double x, void* params);		// regular adaptive integration 2
double func_for_QAQIU_adapt_int1(double x, void* params);	// adaptive integration over the interval (a, +inf);


