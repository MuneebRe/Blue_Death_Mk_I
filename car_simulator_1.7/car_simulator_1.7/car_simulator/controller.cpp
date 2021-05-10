
#include <cmath>   // math functions
#include <cstdio>  // standard I/O functions
#include <cstring> // string manipulation functions

#include <iostream>  // console stream I/O
#include <fstream>   // file stream I/O
#include <strstream> // string stream I/0
#include <iomanip>   // I/O manipulators

#include <windows.h> // for keyboard input

// user defined functions
#include "timer.h" // for measuring time
#include "rotation.h" // for computing rotation functions
#include "3D_graphics.h" // user functions for DirectX 3D graphics
#include "Serial.h"

#include "graphics.h"

#include "robot.h"

#include "controller.h"

#include "image_transfer7.h"

#include "Camera.h"

#include "BDMK1.h"

extern BDMK1 bdmk1;
void speed_controller(double& v_req);

using namespace std;

const double PI = 4*atan(1.0);

// output file for testing / debugging purposes since 
// console output is not easy in a graphics program
//ofstream fout("output.txt");

// initial location of car
// -> you can start the car on any spot of the track
double X0 = -70.0;
double Y0 = 11;
double THETA_0 = 0.0;

extern robot robot1;

extern bool VFF_Feature;
extern bool steer_feature;
double draw[8];

double plot_velocity_target = 0;
double plot_r_target = 0;
double plot_VFF_theta_delta = 0;

// make global since u_phi is needed by the graphics program
// to draw the angle of the front wheels when they steer
double u_s = 0.0, u_phi = 0.0;	

void reset_ICs();

// note that the following views are availble in the simulation
// by pressing the number keys

// 1 - overhead view (3rd person perspective)
// 2 - driver view (1st person perspective)
// 3 - diagonal view (3rd person perspective)
// 4 - close up overhead driver view (1st person perspective)
// 5 - overhead driver view (1st person perspective)

// note calculate_control_inputs() is analogous to loop()
// in an Arduino program -- it get's exectuted as fast as possible.
// -- ie this isn't like a main

extern bool HIL_wb_wf;

void HIL_Data()
{
	HIL_wb_wf = 1;
	const int nb_inputs = 8;
	double output[nb_inputs];
	reset_bin(nb_inputs);
	get_from_Serial(output, nb_inputs);


	robot1.y[1] = output[6];
	robot1.y[2] = output[7];
	robot1.u[1] = output[4];
}

void Sim_Step_Data()
{
	HIL_wb_wf = 1;
	const int nb_inputs = 10;
	double output[nb_inputs];
	reset_bin(nb_inputs);
	get_from_Serial(output, nb_inputs);


	robot1.y[1] = output[8];
	robot1.y[2] = output[9];
	robot1.u[1] = output[6];
}

void input_to_buffer(double u_s, double u_t, double u_phi)
{
	ofstream bout;
	//if (bout.is_open()) return;
	bout.open("../../../Serial/input.bin", ios::binary);

	char p_buffer_in[12];
	char* p;
	float* pf;
	p = p_buffer_in;

	pf = (float*)p;
	*pf = u_s;
	p += sizeof(float);

	pf = (float*)p;
	*pf = 0.0;
	p += sizeof(float);

	pf = (float*)p;
	*pf = u_phi;

	bout.write(p_buffer_in, 12);
	bout.close();
}

void calculate_control_inputs()
{	

	// set state variables and outputs each time in the control loop

	double im = robot1.x[1]; // motor current im (A)
	double wm = robot1.x[2]; // motor speed wm (rad/s)
	double theta = robot1.x[3]; // motor angle, theta (rad)	
	double vf = robot1.x[4]; // forward velocity vf (m/s)
	double xf = robot1.x[5]; // forward position xf (m)
	double theta_c = robot1.x[6]; // robot angle theta_c (rad)
	double xc = robot1.x[7]; // robot x position xc (m)
	double yc = robot1.x[8]; // robot y position yc (m)	

	double wb = robot1.y[1]; // back wheel velocity wb (rad/s)
	double wf = robot1.y[2]; // front wheel velocity wf (rad/s)
	double r = robot1.y[3]; // slip ratio r
	double mu = robot1.y[4]; // coefficient of tire friction mu

	double t = robot1.t; // simulation time

	bool brake_active = false;
	bool start_acc = true;
	double Rw = 3.2e-2;

	// max steering angle of typical car 
	double phi_max = 35.0/180*3.14159;
	
	// maximum car voltage (speed input) (V)
	double us_max = 12; // 12 V -> 26.3 m/s -> 95 hm/h, 60 mph !	
	
	double ds, dphi;

	static double t_reset = 300;

	static int init = 0;
	
	// initialization section -- gets exectuted once at the beginning
	if( !init ) {
		
		//fout << scientific;
		
		init = 1;
	}
	
	// read inputs from the keyboard ////////////
	
	ds = 2; // V
	dphi = 0.003; // rad
	
	if( KEY(VK_UP) ) {
		//robot1.brake_active = false;
		if( u_s < us_max ) u_s += ds;		
	}
		
	if( KEY(VK_DOWN) ) {
		//robot1.brake_active = false;
		if( u_s > -us_max ) u_s -= ds;
	}
		
	if( KEY(VK_LEFT) ) {
		if( u_phi < phi_max ) u_phi += dphi;
	}

	if( KEY(VK_RIGHT) )	{
		if( u_phi > -phi_max ) u_phi -= dphi;	
	}

	
	double velocity_target = 0;

	if (t >  1) velocity_target = 20.00;
	if (t > 10) velocity_target = 0;
	
	//Muneeb's Version
	//Sim_Step_Data();	//Can be used to make the car run how the sim_step is simulating
	//HIL_Data();		//Can be used to make the car run how the HIL is simulating

	//Set VFF_Feature to 1 / steer_feature to 0 for steering control using graphics. 
	//Set VFF_Feature to 0 / steer_feature to 1 for steering control using linear algebra

	//Muneeb's Version
	VFF_Feature = 0; //Can be slow since Vision processing takes alot more time than dt = 0.001s
					 //If the car's not moving (can happen with slow computer processing speed)
					 //Then disable bdmk1.traction_PID
	bdmk1.VFF_control(VFF_Feature, u_s, us_max, u_phi, phi_max, t, 0.003);

	//Gurvir's Version
	steer_feature = 1;
	bdmk1.steer_control(steer_feature, u_s, us_max, u_phi, phi_max, t, 0.003, xc, yc, draw);

	//Muneeb's Version
	bool speed_PID_switch = 1;
	bdmk1.speed_PID(speed_PID_switch, velocity_target, wf, robot1.Rw, u_s, us_max, t, 0.003);

	//Muneeb's Version
	bool traction_PID_switch = 1;
	bdmk1.traction_PID(traction_PID_switch, u_s, us_max, r, vf, wb, wf, velocity_target, t, 0.003);

	//Muneeb's Version
	bool brake_PID_switch = 1;
	bdmk1.brake_PID(brake_PID_switch, u_s, us_max, r, vf, wb, wf, velocity_target, t, 0.003);

	//Alexandro's Version
	//Early Controller Design
	//bdmk1.acc(u_s, us_max, r, vf, wb, wf, Rw, brake_active, start_acc, velocity_target, t, 0.003);
	//bdmk1.traction_PID_2(u_s, us_max, r, vf, wb, wf, Rw, brake_active, start_acc, velocity_target, t, 0.003);
	//bdmk1.brakes(u_s, us_max, r, vf, wb, wf, Rw, brake_active, start_acc, velocity_target, t, 0.003);

	//Muneeb's Version
	//input_to_buffer(u_s, 0.0, u_phi);	//Discontinued, can use car_simulator
										//as the controller for the controller on the HIL.
										//Makes it possible to control in Real-Time, though with a bit of lag.

	//****Zihuan's version****
	//This PID converts the desired car velocity by converting the desired speed to motor voltage
	//Traction control was not enabled using this PID, it only controls the speed
	//Disable the PID and the key input controls above to see the difference.
	/*
	double t_sim = robot1.t;
	double v_forward;
	if (t_sim < 15) {
		v_forward = 20;
		u_s = v_forward * 12.0 / 26.3;
	}
	if (t_sim > 15 && t_sim < 30) {
		v_forward = 15;
		u_s = v_forward * 12.0 / 26.3;
	}
	if (t_sim > 45 && t_sim < 60) {
		v_forward = 10;
		u_s = v_forward * 12.0 / 26.3;
	}
	if (t_sim > 60) {
		v_forward = 18;
		u_s = v_forward * 12.0 / 26.3;
	}
	speed_controller(u_s);
	*/



	// set inputs in the robot model
	robot1.u[1] = u_s; // motor voltage V(t)
	robot1.u[2] = 0.0; // disturbance torque Td(t)
	robot1.u[3] = u_phi; // steering angle phi (rad)
	
	// file output
	// note: too much output might slow down the controller and simulation
	//fout << t << "," << xc << "," << yc << "," << robot1.u[1] << "," << u_phi << "\n";
	//fout << t << "," << robot1.u[1] << endl;
	
	// how to periodically reset the ICs
	// -- in case you want to perform some repeated tests, etc.
	if( t > t_reset ) {
		reset_ICs();
		//t_reset += 30; // reset 30 seconds later
		t_reset += 300; // reset 30 seconds later

	}

	
}


void reset_ICs()
{
	// reset inputs
	
	u_s = 0.0;
	u_phi = 0.0;		
	
	robot1.u[1] = 0.0; // motor voltage V(t)
	robot1.u[2] = 0.0; // disturbance torque Td(t)
	robot1.u[3] = 0.0; // steering angle phi (rad)
	
	// reset states
	
	robot1.x[1] = 0.0; // motor current im (A)
	robot1.x[2] = 0.0; // motor speed wm (rad/s)
	robot1.x[3] = 0.0; // motor angle, theta (rad)	
	robot1.x[4] = 0.0; // forward velocity vf (m/s)
	robot1.x[5] = 0.0; // forward position xf (m)
	robot1.x[6] = THETA_0; // robot angle theta_c (rad)
	robot1.x[7] = X0; // robot x position xc (m)
	robot1.x[8] = Y0; // robot y position yc (m)	
	
	// reset outputs
	
	robot1.y[1] = 0.0; // back wheel velocity wb (rad/s)
	robot1.y[2] = 0.0; // front wheel velocity wf (rad/s)
	robot1.y[3] = 0.0; // slip ratio r
	robot1.y[4] = 0.0; // coefficient of tire friction mu
	
}

void speed_controller(double& v_req) {
	static double kp, ki, kd;//PID control gain parameter/
	static double e_p;
	double t1 = 0, dt; //initial time, time difference
	static double tp = 0;//previous time
	double u; //PID control output
	static double err, err_p; //control error and previous error
	double err_d = 0.0; //derivative of error
	double err_i = 0.0; //integral of error
	double v_car;
	t1 = robot1.t; //Get real simulation time
	v_car = robot1.x[4]; //Read the sensor / get output from the simualtion
	dt = t1 - tp;
	tp = t1; //save time for next call
	kp = 10.0;
	ki = 6.0;
	kd = 1.5;
	err = v_req - v_car;
	err_d = (err - e_p) / dt;
	e_p = err;
	err_i = err_i + err_i * dt;
	u = kp * err + ki * err_i + kd * err_d; //PID control input
	if (u > 12) u = 12; //cap the maximum voltage
	if (u < -12) u = -12;
	robot1.u[1] = u; // Write the voltage to the control input
}