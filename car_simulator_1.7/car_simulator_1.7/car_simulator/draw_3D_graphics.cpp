
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <windows.h>

// user defined functions

#include "timer.h"
#include "3D_graphics.h"
#include "rotation.h"
#include "graphics.h"
#include "ran.h"

//#include "image_transfer5.h"
#include "image_transfer7.h"

#include "robot.h"
#include "controller.h"

#include "Camera.h"

#include "curve.h"

#include "BDMK1.h"

#include "vision.h"

#include <thread>

const double PI = 4*atan(1.0);

#define SQR(x)	((x)*(x))

// dynamic arrays for storing DirectX vertices
double *XB, *YB, *ZB, *RB, *GB, *BB, *UB, *VB;

// maximum number of vertices in the dynamic arrays
int MAX_VERTICES = 100000;

// default window size
int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

// 1 - use border, 0 - no border
int BORDER_DEFAULT = 1; 

// background colour (R,G,B)
double BACKG[3] = { 0.28, 0.47, 0.63 };

// default min and max viewing distances.
// objects closer than VMIN and farther than VMAX are not drawn (ie cannot be seen).
// note the ratio of VMAX/VMIN should be less than 60,000 for most graphics cards.
double VMIN = 1.0; // units of m (or whatever units you draw your object in)
double VMAX = 10000.0; // units of m

// ambient light color (R,G,B)
double AMBIENT[3] = { 0.3, 0.3, 0.3 };

// flag to turn directional lights on/off (note: there are 3 lights)
int LIGHT[3] = {1, 1, 1}; // 1 - on, 0 - off

// light direction
double DIR1[3] = { -1.0, -1.0, -1.0 }; // light #1
double DIR2[3] = { 1.0, 0.0, 0.0 }; // light #2
double DIR3[3] = { 0.0, 0.0, 1.0 }; // light #3

// light colour (RGB)
double COLOUR1[3] = { 0.7, 0.7, 0.7 }; // light #1
double COLOUR2[3] = { 0.5, 0.5, 0.5 }; // light #2
double COLOUR3[3] = { 0.3, 0.3, 0.3 }; // light #3

// FOV - field of view (PI/4 rad is a typical value)
double FOV = PI/4;

int VIEW_2D = 0; // 2D_view mode flag
// 0 - use 3D view
// 1 - X-Y coordinates
// 2 - X-Z coordinates
// 3 - Y-Z coordinates

// width of the 2D view window in global coordinates (m)
double WIDTH_2D = 20.0;

// height of the 2D view window in global coordinates (m)
double HEIGHT_2D = 15.0;

// make 2D correspond to pixel coord
//double WIDTH_2D = (double)WINDOW_WIDTH;
//double HEIGHT_2D = (double)WINDOW_HEIGHT;

// sync fps with screen refresh rate (0 - no sync, 1 - sync)
int SYNC_FPS_WITH_SCREEN_REFRESH = 0;

// 1 - on, 0 - off
// note: don't turn this setting on with SYNC_FPS
int SWAP_EFFECT_FLIP = 0;

extern double X0_VIEW;
extern double Y0_VIEW;
extern double Z0_VIEW;
extern double THETA_VIEW;
extern double draw[8];
static int steer_init = 0;

using namespace std;

image b;
Camera view(true, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB_IMAGE, true, 1);
BDMK1 bdmk1;
bool VFF_Feature = 0;
bool steer_feature = 0;

static ofstream fout("timing.txt");
//static ofstream fout1("gurvtesting.txt");

extern double X0;
extern double Y0;
extern double THETA_0;

extern double u_phi;

robot robot1(X0,Y0,THETA_0);

void draw_car(double x_c, double y_c, double z_c, double yaw_c, double pitch_c,
	double roll_c, double th_front, double th_back, double th_steer);

double global_yaw;
bool s_finished = false;

void DoWork()
{
	using namespace std::literals::chrono_literals;
	
	if (s_finished == false)
	{
	
		//std::this_thread::sleep_for(3s);

		double pt_i;
		double pt_j;

		//view.acquire();
		
		view.set_processing(6);		//Run filter for blue, orange, green and red
		view.processing();			//to find centroid location for each
		pt_i = view.get_ic();	//And put them in this array
		pt_j = view.get_jc();	//For each color
		
		view.set_processing(1);		//Run filter for blue, orange, green and red
		view.processing();			//to find centroid location for each

		bdmk1.label_nb_1 = view.label_at_coordinate(pt_i, pt_j);

		view.track_object(pt_i, pt_j);
		
		bdmk1.set_coord(pt_i, pt_j);
		bdmk1.set_theta(global_yaw);


		bdmk1.highlight_view(view);
		/*
		//view.set_processing(13);
		//view.processing();
		*/
		draw_point_rgb(view.rgb, pt_i, pt_j, 255, 0, 0);
		
		//view.view();

		s_finished = true;
	}
		
		

	
}

void draw_3D_graphics()
{
	static int init = 0; // initialization flag
	static double tc0; // initial clock time
	
	double tc; // current clock time
	double x, y, z;
	double yaw, pitch, roll;
	
	static double th_front = 0.0, th_back = 0.0, th_steer = 0.0;

	static double t_sim = 0.0;
	double dt_sim;	

	// initalization section
	if( !init ) {
		tc0 = high_resolution_time(); // initial clock time (s)

		activate_vision();

		init = 1;
	} // end of initialization section

	// calculate control inputs
	calculate_control_inputs();
	
	//bdmk1.VFF_control(robot1.u[1], robot1.u[3]);

	// read clock time
	tc = high_resolution_time() - tc0;
	
	dt_sim = 0.001; // simulation time step

	// real-time simulation of robot
	while( t_sim < tc ) {
		robot1.sim_step(dt_sim,robot1);
		
		t_sim += dt_sim;
	}

	th_back = robot1.x[3]; // w
	
	// v = wf * Rw -> x = theta_f * Rw
	// theta_f = x / Rw
	th_front = robot1.x[5] / robot1.Rw; 
	
	// get variables to draw car from simulation
	yaw = robot1.x[6];
	pitch = 0.0;
	roll = 0.0;
	
	x = robot1.x[7];
	y = robot1.x[8];
	z = 0.0;
	
	th_steer = u_phi;
	
	if (steer_feature == 0) {	//This will only occur if steering without VFF is disabled

		double pixels_per_m = 21.15; // scale of track images
		double x_offset = -1690.5;
		double s_begin = -4.00e2;// 3.7500000e+02; // begining of curve
		//double s_end = 2.7275000e+04; // end of curve
		double s_end = (2.7275000e+04 - s_begin) / 2.00000;

		for (double s = s_begin; s < s_end; s = s + 150)
		{
			double x, y, xd, yd, xdd, ydd, theta;
			curve(s, x, y, xd, yd, xdd, ydd);

			// adjust x offset between full and half track

			x = x + x_offset;

			// convert x and y to m
			x = x / pixels_per_m;
			y = y / pixels_per_m;
			theta = atan2(yd, xd);
			draw_box(x, y, -0.35, theta, 0, 0, 10, 12, 0.025, 0, 0, 0, 1);

			//if (s == s_begin + 1500) draw_box(x, y + 19 , -0.35, theta, 0, 0, 10, 12, 0.025, 255, 255, 255, 1);

			//draw_box(x, y, -0.34, theta, 0, 0, 3, 0.5, 0.025, 255, 255, 255, 1);
		}
	}
	else if (steer_feature == 1) {
		bdmk1.steer_index = 0;
		double pixels_per_m = 21.15; // scale of track images
		double x_offset = -1690.5;
		double s_begin = -4.00e2;// 3.7500000e+02; // begining of curve
		//double s_end = 2.7275000e+04; // end of curve
		double s_end = (2.7275000e+04 - s_begin) / 2.00000;

		for (double s = s_begin; s < s_end; s+=5)
		{
			double x, y, xd, yd, xdd, ydd, theta;
			curve(s, x, y, xd, yd, xdd, ydd);

			// adjust x offset between full and half track

			x = x + x_offset;

			// convert x and y to m
			x = x / pixels_per_m;
			y = y / pixels_per_m;
			theta = atan2(yd, xd);
			//draw_box(x, y, -0.35, theta, 0, 0, 2, 2, 0.025, 255, 255, 255, 1);
			//draw_box(draw[0], draw[1], -0.35, yaw, 0, 0, 2, 2, 0.025, 255, 255, 0, 1);
			//draw_box(draw[2], draw[3], 0, draw[6], 0, 0, 1, 1, 0, 0, 255, 0, 1);
			//draw_box(draw[4], draw[5], 0, draw[7], 0, 0, 1, 1, 0.0, 255, 0, 0, 1);
			if (steer_init == 0) {
				bdmk1.steer_x[bdmk1.steer_index] = x;
				bdmk1.steer_y[bdmk1.steer_index] = y;
				bdmk1.steer_theta[bdmk1.steer_index] = theta;
				bdmk1.steer_index += 1;
			}
		}
	}
	steer_init = 0;
		// draw car
		draw_car(x, y, z, yaw, pitch, roll, th_front, th_back, th_steer);

		global_yaw = yaw;

		if (VFF_Feature == 1)
		{
			std::thread worker(DoWork);

			if (worker.joinable())
			{
				worker.join();
			}

			if (s_finished == true)
			{
				view.view();
				s_finished = false;
				view.acquire();
			}
		}
	
	
	/*
	view.acquire();

	view.set_processing(6);		//Run filter for blue, orange, green and red
	view.processing();			//to find centroid location for each
	pt_i = view.get_ic();	//And put them in this array
	pt_j = view.get_jc();	//For each color

	view.set_processing(1);		//Run filter for blue, orange, green and red
	view.processing();			//to find centroid location for each

	bdmk1.label_nb_1 = view.label_at_coordinate(pt_i, pt_j);

	view.track_object(pt_i, pt_j);

	bdmk1.set_coord(pt_i, pt_j);
	bdmk1.set_theta(yaw);
	

	//bdmk1.highlight_view(view);

	//view.set_processing(13);
	//view.processing();

	draw_point_rgb(view.rgb, pt_i, pt_j, 255, 0, 0);

	view.view();
	*/
}


void draw_car(double x_c, double y_c, double z_c, double yaw_c, double pitch_c,
	double roll_c, double th_front, double th_back, double th_steer)
{
	// note: static is used here so the constructor only gets called 
	// once and also so the mech constructor gets called after
	// winmain starts the graphics library
	
	static mesh body("porsche_body.cmo"); 
	static mesh wheel_l("porsche_wheel_left.cmo");
	static mesh wheel_r("porsche_wheel_right.cmo");	
	static mesh track("half_track.cmo");
	static mesh sky("sky.cmo");

	double yaw, pitch, roll, scale;
	double R1[3+1][3+1], R2[3+1][3+1];
	double R[3+1][3+1], rho[3+1], dP[3+1], P[3+1], T[3+1];
	int i;

	// view parameters
	static double eye_point[3+1], lookat_point[3+1], up_dir[3+1], fov;
	static double eye_point_l[3+1], lookat_point_l[3+1], up_dir_l[3+1];
	
	static int init = 0, view_flag;

	// initialization section
	if( !init ) {		
		view_flag = 5; // view ID number
		fov = 3.14159/4; // field of view
		init = 1;
	}

	// size / scale for the car
	scale = 1.537;
	
	body.Scale = scale;
	wheel_l.Scale = scale;
	wheel_r.Scale = scale;

	// position of car center T in global coord
	T[1] = x_c;
	T[2] = y_c;
	T[3] = z_c;

	// car rotation
	yaw = yaw_c;
	pitch = 0.0;
	roll = 0.0;
	euler_to_rotation(yaw,pitch,roll,R1);
	
	// set different view points depending on key input
	if( KEY('1') || ( view_flag == 1 ) ) {
		eye_point[1] = 0.0; eye_point[2] = 0.0; eye_point[3] = 140.0;
		lookat_point[1] = 0.0; lookat_point[2] = 0.0; lookat_point[3] = 0.0;
		up_dir[1] = 0.0; up_dir[2] = 1.0; up_dir[3] = 0.0;
		view_flag = 1;
	}
	
	// 1st person perspective
	if( KEY('2') || ( view_flag == 2 ) ) {
		
		// set view parameters in local coordinates
		eye_point_l[1] = 0;
		eye_point_l[2] = 0;
		eye_point_l[3] = 3;
		lookat_point_l[1] = 1; // lookat point in the local x-direction
		lookat_point_l[2] = 0;
		lookat_point_l[3] = 3;
		up_dir_l[1] = 0.0;
		up_dir_l[2] = 0.0; 
		up_dir_l[3] = 1.0; // in the local z direction

		// multiply a matrix times a vector, b = A*x (from rotation.h/cpp)
		// calculate b = R1*pl
		multiply_Ax(R1,eye_point_l,eye_point);
		multiply_Ax(R1,lookat_point_l,lookat_point);
		multiply_Ax(R1,up_dir_l,up_dir);

		// add T to R1*pl for eye_point_g and lookat_point_g
		// ie Pg = T + R1*pl
		for(i=1;i<=3;i++) eye_point[i] += T[i];
		for(i=1;i<=3;i++) lookat_point[i] += T[i];

		view_flag = 2;
	}	
	
	// set different view points depending on key input
	if( KEY('3') || ( view_flag == 3 ) ) {
		eye_point[1] = -60.0; eye_point[2] = -100.0; eye_point[3] = 60.0;
		lookat_point[1] = 0.0; lookat_point[2] = 0.0; lookat_point[3] = 0.0;
		up_dir[1] = 0.0; up_dir[2] = 0.0; up_dir[3] = 1.0;
		view_flag = 3;
	}

	// set different view points depending on key input
	if( KEY('4') || ( view_flag == 4 ) ) {
		eye_point[1] = T[1]; eye_point[2] = T[2]; eye_point[3] = T[3] + 30;
		lookat_point[1] = T[1]; lookat_point[2] = T[2]; lookat_point[3] = T[3];
		up_dir[1] = 0.0; up_dir[2] = 1.0; up_dir[3] = 0.0;
		view_flag = 4;
	}

	// set different view points depending on key input
	if( KEY('5') || ( view_flag == 5 ) ) {
		eye_point[1] = T[1]; eye_point[2] = T[2]; eye_point[3] = T[3] + 50;
		lookat_point[1] = T[1]; lookat_point[2] = T[2]; lookat_point[3] = T[3];
		up_dir[1] = 0.0; up_dir[2] = 1.0; up_dir[3] = 0.0;
		view_flag = 5;
	}

	//////////////////////////////////////

	// set view matrices
	set_view(eye_point,lookat_point,up_dir);

	// set an additional light
	// maybe looks better but is slower
//	set_light(1,1.0,-2.0,3.0,1.0,1.0,1.0,1);

	// end of set view point code ////////////////

	// draw car body
	body.draw(T[1],T[2],T[3],yaw,pitch,roll);	

	// draw back wheels ///////////////////////////////////////
	
	// back wheel rotation with respect to body

	yaw = 0.0;
	pitch = th_back;
	roll = 0.0;
	euler_to_rotation(yaw,pitch,roll,R2);

	// calculate overall rotation matrix of the wheel R = R1 * R2
	multiply_AB(R1,R2,R);

	// calculate Euler angles for R
	rotation_to_euler(R,yaw,pitch,roll);

	// rho2 - position of back left wheel in car local coord
	rho[1] = -0.675*scale;
	rho[2] = 0.5*scale;
	rho[3] = -0.3*scale;

	// calculate the wheel center position in global coord
	// P = T1 + R1*T2 = T1 + R1*pho
	multiply_Ax(R1,rho,dP);
	for(i=1;i<=3;i++) P[i] = T[i] + dP[i];

	// draw back left wheel
	wheel_l.draw(P[1],P[2],P[3],yaw,pitch,roll); 

	// rho2 - position of back right wheel in car local coord
	rho[1] = -0.675*scale;
	rho[2] = -0.5*scale;
	rho[3] = -0.3*scale;

	// calculate the wheel center position in global coord
	// P = T1 + R1*T2 = T1 + R1*pho
	multiply_Ax(R1,rho,dP);
	for(i=1;i<=3;i++) P[i] = T[i] + dP[i];

	// draw back right wheel
	wheel_r.draw(P[1],P[2],P[3],yaw,pitch,roll); 

	// draw front wheels //////////////////////////////////////

	// front wheel rotation with respect to body

	yaw = th_steer;
	pitch = th_front;
	roll = 0.0;
	euler_to_rotation(yaw,pitch,roll,R2);

	// calculate overall rotation matrix of the wheel R = R1 * R2
	multiply_AB(R1,R2,R);

	// calculate Euler angles for R
	rotation_to_euler(R,yaw,pitch,roll);

	// rho2 - position of front left wheel in car local coord
	rho[1] = 0.8*scale;
	rho[2] = 0.45*scale;
	rho[3] = -0.3*scale;

	// calculate the wheel center position in global coord
	// P = T1 + R1*T2 = T1 + R1*pho
	multiply_Ax(R1,rho,dP);
	for(i=1;i<=3;i++) P[i] = T[i] + dP[i];

	// draw front left wheel
	wheel_l.draw(P[1],P[2],P[3],yaw,pitch,roll); 

	// rho2 - position of front right wheel in car local coord
	rho[1] = 0.8*scale;
	rho[2] = -0.45*scale;
	rho[3] = -0.3*scale;

	// calculate the wheel center position in global coord
	// P = T1 + R1*T2 = T1 + R1*pho
	multiply_Ax(R1,rho,dP);
	for(i=1;i<=3;i++) P[i] = T[i] + dP[i];

	// draw front right wheel
	wheel_r.draw(P[1],P[2],P[3],yaw,pitch,roll); 

//	sky.Scale = 80.0;
//	sky.draw(0.0,0.0,-80.0,0.0,0.0,3.14159/2);

//	track.Scale = 2;
//	track.draw(0.0,0.0,-1.2,0.0,0.0,0.0);

	// draw sky
	sky.Scale = 80.0;
	sky.Roll_0 = 3.14159/2;
	sky.Y_0 = -1.0;
	sky.draw(0.0,0.0,0.0,0.0,0.0,0.0);

	// draw track
	track.Scale = 2;
	track.Z_0 = -0.35;
	track.draw(0.0,0.0,-0.5,0.0,0.0,0.0);

}

