
// simulation of wheel-slip model using the magic tire formula
// https://www.mathworks.com/help/physmod/sdl/ref/tiremagicformula.html
// https://www.mathworks.com/help/physmod/sdl/ref/tireroadinteractionmagicformula.html

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// sign macro function
#define SIGN(a) ( (a) >= 0.0 ? 1.0 : -1.0 )

double calculate_mu(double r);

double calculate_mu_bw(double r);

int main()
{
	const int N = 2; // number of state variables
	int i;
	double t; // current time (seconds)
	double x[N+1];  // state vector x[N](t)
	double xd[N+1]; // derivative vector at time t
	double dt = 0.001; // time step (s)
	double tf; // final time (s)
	ofstream fout;

	double J,M,R,b,g,tau,S,v,w,mu,mu_bw;
	double tol = 1.0e-10;

	// model parameters
	J = 1.0;
	M = 1.0;
	R = 1.0;
	b = 0.5;
	g = 9.8;

	fout.open("sim.txt"); // output file

	if(!fout) {
		cout << "\nfile open error";
		return 1;
	}

	t = 0.0; // initial time (s)

	// initial conditions (IC)
	x[1] = 0.0; // w(0)
	x[2] = 0.0; // v(0)

	tf = 50.0; // final time (s)

	fout << scientific;

	fout.precision(5);

	fout << "%t            \tw            \tv            \tu            \tS           \tmu\n";

	cout << "\nsimulation starting ...\n";

	while(t < tf) {

		// save time and states into a file
		fout << t;
		for(i=1;i<=N;i++) fout << "\t" << x[i];

		// calculate the derivative vector at time t

		// calculate slip
		w = x[1];
		v = x[2];

		// checking for / 0 is not needed because of tol
		S = ( w*R - v ) / ( fabs(v) + tol );

		mu = calculate_mu_bw(S);
//		mu_bw = calculate_mu_bw(S); // simplified friction model from myself (BW)

		// set input tau
		// input torque -- high
		tau = 50.0; // Nm

		// save input torque
		fout << "\t" << tau;

		// save slip ratio
		
		// limit slip ratio if too large so plot scale isn't too large
		if( fabs(S) > 10 ) S = 10*SIGN(S);		
		fout << "\t" << S;
		
		// save friction coefficient mu
		fout << "\t" << mu;

		// wd = ( tau - b*w - R*M*g*mu ) / J
		xd[1] = ( tau - b*w - R*M*g*mu ) / J;

		// vd = g*mu
		xd[2] = g*mu; 

		// calculate x(t+dt)
		// Euler 1st order simulation
		for(i=1;i<=N;i++) x[i] = x[i] + xd[i]*dt;

		// lock wheels and brake test
		if( t > 25 ) {
			x[1] = 0.0; // w = 0
		}

		t = t + dt; // increment time

		if(t<tf) fout << "\n";

	}

	fout.close();

	cout << "\ndone.\n";
	getchar();

	return 0;
}

// magic formula for tire friction

// F = N*D*sin(C*atan(B*r-E*(B*r−atan(B*r))))

// coefficient table from
// https://www.mathworks.com/help/physmod/sdl/ref/tiremagicformula.html
// https://www.mathworks.com/help/physmod/sdl/ref/tireroadinteractionmagicformula.html

// Surface	B		C		D		E
// Dry		10		1.9		1		0.97
// Wet		12		2.3		0.82	1
// Snow		5		2		0.3		1
// Ice		4		2		0.1		1

double calculate_mu(double r)
{
	double mu;
	double B, C, D, E, rmax;
	
//	B=10; C=1.9; D=1; E=0.97; // dry
	B=12; C=2.3; D=0.82; E=1; // wet	
//	B=5; C=2; D=0.3; E=1; // snow
//	B=4; C=2; D=0.1; E=1; // ice
	
	// maximum slip ratio for model -- prevents too much extrapolation
	rmax = 10.0;

	// limit range of r
	if( r > rmax )  r = rmax;
	if( r < -rmax ) r = -rmax;

	mu = D*sin( C*atan( B*r-E*(B*r-atan(B*r)) ) );	
	
	return mu;
}


double calculate_mu_bw(double r)
// BW model
// similar to Burckhardt model but with added term a2 for dip in curve
{
	double mu;
	double a1, a2, a3, c1, c2, rmax;

	// note a2 is mainly for wet road dip in curve

	// the ai parameters can be readily estimated off-line or on-line
	a1 = 1.63; a2 = -0.9; a3 = -0.1; rmax = 1.0; // wet
	
	// rmax is the maximum slip ratio for model -- prevents too much 
	// extrapolation from the a3 term

	// normally c1 and c2 should be constant for different road conditions
	c1 = -27;
	c2 = -15; 

	// limit range of r
	if( r > rmax )  r = rmax;
	if( r < -rmax ) r = -rmax;
	
	if( r < 0 ) {
		r = fabs(r);
		mu = -( a1 * (1-exp(c1*r)) + a2 * (1-exp(c2*r)) + a3 * r );
	} else {
		mu = a1 * (1-exp(c1*r)) + a2 * (1-exp(c2*r)) + a3 * r;	
	}
	
	return mu;
}

