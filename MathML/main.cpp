#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alglib/integration.h"

using namespace alglib;
void int_function_1_func(double x, double xminusa, double bminusx, double &y, void *ptr)
{
	// this callback calculates f(x)=exp(x)
	y = sin(x);
}

int main(int argc, char **argv)
{
	//
	// This example demonstrates integration of f=exp(x) on [0,1]:
	// * first, autogkstate is initialized
	// * then we call integration function
	// * and finally we obtain results with autogkresults() call
	//
	double a = 0;
	double b = 3;
	autogkstate s;
	double v;
	autogkreport rep;

	autogksmooth(a, b, s);
	alglib::autogkintegrate(s, int_function_1_func);
	autogkresults(s, v, rep);

	printf("%.15f\n", double(v)); // EXPECTED: 1.7182
	return 0;
}