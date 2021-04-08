
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "nrutil.h"
#include "recipes.h"
#include "cubic_spline.h"

#include "curve.h"

using namespace std;

int main()
{
	double t, x = 0, y = 0, t_begin, t_end;
	double xd, yd, xdd, ydd; // 1st and 2nd derivatives of x, y
	ofstream fout("output.txt");
	
	fout << scientific;
	fout.precision(6);
	
	t_begin = 0.0; // begining of curve
	t_end = 26900.0; // end of curve
	
	for( t = t_begin; t < t_end; t += 7.7 ) {
		
		// calculate curve equation x(t), y(t) for a given t
		// note t is curve length from begining, not time
		curve(t,x,y,xd,yd,xdd,ydd);
		
		fout << t << " " << x << " " << y << "\n";
	}

	cout << "\ndone.\n";

	return 0;
}

