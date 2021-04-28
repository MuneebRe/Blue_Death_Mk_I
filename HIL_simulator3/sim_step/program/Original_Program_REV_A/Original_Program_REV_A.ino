
// version 1.1 -- replace calculate_mu with calculate_mu_bw
// -> reduces sim_step time from 1000 us to 835 us

// version 1.2 -- change / to * for constants
// -> reduces sim_step time from 835 us to 700 us

// sign macro function
#define SIGN(a) ( (a) >= 0.0 ? 1.0 : -1.0 )

void sim_step(float &t, float x[], float u[], float dt, float y[]);

const int NS = 5; // number of state variables (order)
const int MS = 2; // number of inputs
const int PS = 2; // number of outputs (NOTE: dropped S and mu here -- note needed)

float calculate_mu_bw(float r);

void setup() { // note: setup is analogous to main not loop

	int i;
	float t; // current time (seconds)	
	float dt; // time step (s)
	float tf; // final time (s)	
	float x[NS+1];  // state vector
	float u[MS+1]; // input vector u
	float y[PS+1]; // outputs of interest for plotting
	
	// Serial startup sequence below ensures reliable / predictable startup /////
	char ch;
	
	Serial.begin(2000000);
	
	// wait until Serial Monitor or save_serial program is started
	while( !Serial ) delay(1);
	
	// note: the following code is the best way to start an Arduino 
	// program in my opinion since you can control the start time exactly
	// and you can wait until you are ready to watch the experiment.
	while(1) {
		if( Serial.available() > 0 ) { // is there incoming data
			ch = Serial.read(); // read one character / byte
			if( ch == 's' ) break;
		}
		delay(10); // leave some time for printing message above, etc.
	}
	
	// give some time for user to look at robot
	delay(3000);
	
	// NOTE: you can try using the following line here to make sure each line 
	// is completely output before continuing -- this might make the 
	// output more reliable
	// Serial.flush() // completely write serial output before continuing		
	
	// end of Serial startup sequence ///////////////////////////////////	
	
	// label the columns (the % is so matlab plotting m-files ignore this line)
	Serial.print("%time(s),x1,x2,x3,x4,x5,u1,u2,y1,y2\n");

	// 3. set simulation parameters
	// - try an initial guess for dt (say 0.01) and then run the simulation.
	// - keep reducing dt until the solution / output plots converge (stop changing)
	dt = 0.001; // time step

	// dt should be at least 10x smaller than the smallest
	// time constant for the system

	tf = 20.0; // final simulation time (s)

	// first call to sim_step(...) just sets ICs and parameters
	sim_step(t,x,u,dt,y);

	// simulation loop
	while(t < tf) {
		
		// save time and state variables and input into a file
		Serial.print(t,5);
		for(i=1;i<=NS;i++) {
			Serial.print(",");
			Serial.print(x[i],5);		
		}
		
		// calculate control inputs
		u[1] = 12.0; // motor voltage V(t)
		u[2] = 0.0; // disturbance torque Td(t)
		for(i=1;i<=MS;i++) {
			Serial.print(",");
			Serial.print(u[i],5);		
		}
		
		// Euler simulation step of dt
		sim_step(t,x,u,dt,y);
	
	
		// outputs of interest for plotting
		for(i=1;i<=PS;i++) {
			Serial.print(",");
			Serial.print(y[i],5);				
		}	
	
		// each row represents a given time
		if( t < tf ) Serial.print("\n"); // move on to next row
		
		delay(1);

	} // end while

	// stop save serial program
	Serial.print("#");

	delay(1000);
	exit(0); // not using loop()
}


void loop()
{
	// not used
}


void sim_step(float &t, float x[], float u[], float dt, float y[])
{
	int i;
	static float xd[NS+1]; // derivative vector at time t	
	
	// note static variables are used to store variables 
	// that need to be remembered between function calls.
	
	// 1. set model parameters (m, k, b, etc.)
	static float L, R, kb; // electrical model parameters
	static float J, km, b, fc; // mechanical model parameters
	static int init = 0; // initialization flag
	
	// new parameters for traction model
	static float m, Rw, g, Q, GR;
	float mu, Ft, wm, v, r, wb, wf;
	float tol = 1.0e-10;
	
	// new constants to speed up the simulation
	static float Fn, GR_inv, L_inv, J_inv, m_inv;	
	
	// initialization section (gets executed only once
	// the first time you call the function)
	if( !init ) {
		
		L = 0.003; ///
		R = 0.141; ///
		// note due to energy conservation kb = km always
		kb = km = 0.00574; /// 
		J = 0.0001; /// this might also include the load inertia (wheel, gears, etc.)
		b = 3.97e-6; ///
		fc = 0.0; /// 0.008 max
		
		// new parameters for traction model
		m = 1.136; /// (kg) total car mass
		Rw = 3.2e-2; /// (m) 3.2 cm tire radius
		g = 9.8; /// g (m/s^2)
		Q = 0.37; /// rear weight distribution
		GR = 2.5; /// gear ratio
				
		// new constants to speed up the simulation ///////
		
		Fn = m*g*Q; /// normal force on back wheels	(assume constant)
		
		// constants to replace slow / with faster *
		GR_inv = 1/GR;
		L_inv = 1/L;
		J_inv = 1/J;
		m_inv = 1/m;
		
		////////////////////////////////////
	
		// 2. set initial conditions (IC)
		t = 0.0; // initial time	
		x[1] = 0.0; // initial current, i (A)
		x[2] = 0.0; /// initial velocity of motor, wm (rad/s)
		x[3] = 0.0; // initial angle, theta (rad)	

		// new states for the traction model
		x[4] = 0.0; // initial forward velocity v (m/s)
		x[5] = 0.0; // initial x position (m)

		init = 1; // initialization complete
		
		return; // first call to sim_step(...) just sets ICs and parameters
		
	} // end of initialization section
	
	// 4. calculate the derivative vector xd at time t
	
	// gear equations ////////
	/// wb = 1 / GR * wm
	/// tau_b = GR * tau_m	
	
	// calculate slip ratio r

	wm = x[2]; /// motor angular velocity, wm (rad/s)
	wb = GR_inv * wm; /// back wheel angular velocity, wb (rad/s)
	
	v = x[4]; // forward velocity v (m/s)

	// checking for / 0 is not needed because of tol
	r = ( wb * Rw - v ) / ( fabs(v) + tol ); ///
	
	// calculate friction coefficient
	mu = calculate_mu_bw(r); ///
	
	// outputs of interest for plotting
	y[1] = wb; // back wheel velocity (rad/s)
	
	// calculate front wheel angular velocity wf
	// v = wf * Rw -> wf = v / Rw
	wf = v / Rw;
	
	y[2] = wf; // front wheel velocity (rad/s) 		
	
	// calculate tire force
	Ft = mu * Fn;
	
	/// tau_b = GR * tau_m -> tau_m = tau_b / GR
	
	// DC motor equations (modified for tire torque Rw*Ft)
	xd[1] = (-x[1]*R - kb*x[2] + u[1]) * L_inv; // di/dt
	xd[2] = (km*x[1] - b*x[2] - fc*SIGN(x[2]) - (Rw*Ft)*GR_inv - u[2]) 
				* J_inv; /// dw/dt
	xd[3] = x[2]; // dth/dt = w

	// note that combining state variable equation models
	// normally requires exchange / sharing of coupling 
	// terms / variables between both sets of equations
	// -- in this case the tire force Ft
	
	// algebraic constrants may also occur with 
	// resulting coupling forces (lagrange multipliers, etc.)
	// -- requires differential algebraic equation (DAE) solvers 

	// new state-variable equations for the traction model
	xd[4] = Ft * m_inv; // dv/dt
	xd[5] = x[4]; // dx/dt = v

	// 5. apply Euler's equation, x = x + dx, note x is a vector
	// this part is always the same
	// but calculating xd will normally be different
	for(i=1;i<=NS;i++) x[i] = x[i] + xd[i]*dt; 
		
	t = t + dt; // increment time
	
}


float calculate_mu_bw(float r)
// BW model
// similar to Burckhardt model but with added term a2 for dip in curve
{
	float mu;
	float a1, a2, a3, c1, c2, rmax;

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
