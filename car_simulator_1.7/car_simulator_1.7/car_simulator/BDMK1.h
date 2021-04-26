#pragma once
# define M_PI           3.14159265358979323846  /* pi */

/*
#include "Neural_Network/NeuroNet.h"
#include "Neural_Network/Input.h"
#include "Neural_Network/Hidden.h"
#include "Neural_Network/Output.h"
*/

#include "Camera.h"

class BDMK1
{
public:
	int pw_l, pw_r, pw_laser, laser;

	bool collision_state[4];
	bool collision_t_flag[4];
	double collision_t1[4];
	double collision_t2[4];
	double collision_dt[4];
	double collision_dt_target[4];

	bool net_mem[7];
	double net_out[3];

	int label_nb_1;
	int label_nb_2;


private:
	double x1, x2, dx, ddx;
	double y1, y2, dy, ddy;
	double theta, dtheta, ddtheta;
	double theta_target1, theta_target2;
	double target_delta1, target_delta2;
	double trigger_range;
	bool target_state;
	bool state_laser;
	bool state_dir[2];	//Trigger to determine which side enemy located
	bool flag_reset;
	bool is_there_obstacle;
	double distance_enemy1;

	int Lx[8];
	int Ly[8];
	int LL[8];
	int Ln[8];

	int Ax[8];
	int Ay[8];
	double AF[8];
	double distance_log[8];

	//Neural_Net* topology;
	double trial_timer1, trial_timer2, trial_dt;

	double VFF_theta;
	double VFF_mag;

public:
	BDMK1();
	//void init_neural();
	void manual_set(int& pw_l, int& pw_r, int& pw_laser, int& laser);
	void set_coord(double x1, double y1);
	void collision_points(Camera& view);
	void check_collision(int arrx[], int arry[], Camera& view, int i);
	double get_theta() { return theta; }
	void set_theta(double theta);
	void find_target(BDMK1 enemy);
	double get_x1() { return x1; }
	double get_y1() { return y1; }
	double get_x2() { return x2; }
	double get_y2() { return y2; }
	void calculate_theta(double x1, double y1, double x2, double y2, double& theta);
	void theta_target_delta_fix(double target_theta, double& target_delta, int& aim_dir);
	bool get_reset_state() { return flag_reset; }

	void distance_sensor(Camera& view, BDMK1 enemy);
	void distance_input(int arrx[], int arry[], Camera& view, int i);
	void is_obstacle_before_enemy(int arrx[], int arry[], BDMK1 enemy, Camera& view);
	void label_enemy(Camera& view, BDMK1 enemy);
	void fill_wheel_void(Camera& view);

	
	//void NeuroLearn(int& pw_l, int& pw_r, int& laser, int& trial_number);

	void scout(int& pw_l, int& pw_r, int& pw_laser, int& laser);
	void attack(int& pw_l, int& pw_r, int& pw_laser, int& laser);

	void highlight_view(Camera& view);
	void hide_shadows(int arrx[], int arry[], Camera& view, double theta_index, int& radar_radius, int radius_limit, int radius_jump);
	void VFF_section_modifier(double theta_index, double offset, double range, int& radius_limit, int limit_val, double& multiplier, double multiplier_val);

	void VFF_control(double& us, double& phi);

	~BDMK1();
};