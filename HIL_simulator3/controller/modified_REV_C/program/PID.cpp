#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

bool acceleration_trigger = 0;
float plot_r_target = 0;
float plot_velocity_target = 0;

const float velocity_target_tol = 2.0;

void speed_PID(bool is_enable, float target_vf, float wf, float Rw, float& u_s, float us_max, float t, float interval)
{
  static double time1 = 0;
  static double time2 = 0.1;
  double time_delta;
  //time2 = t;
  time2 = micros()*1e-6;
  time_delta = time2 - time1;
  if (time_delta < interval) return;
  time1 = time2;

  double kc = 1;
  double kp_PID = 0.46;//0.46;
  double kd_PID = 0;
  double ki_PID = 0;

  //Max speed - 26.3116 m/s at 12 V

  double error = 0;
  static double old_error = 0;
  double error_dot = 0;
  double int_error = 0;


  error = (target_vf*2 - wf*Rw);
  error_dot = (error - old_error) / time_delta;
  int_error = int_error + error * time_delta;
  if (is_enable == 1) u_s = (kp_PID * error + ki_PID * int_error + kd_PID * error_dot);
  u_s = u_s * kc;

  if (u_s > us_max) u_s = us_max;
  if (u_s < -us_max) u_s = -0;

  old_error = error;

  plot_velocity_target = target_vf;
}

void traction_PID(bool is_enable, float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval)
{
  acceleration_trigger = 0;

  if (fabs(wb - wf) < velocity_target_tol && velocity_target > vf) acceleration_trigger = 1;

  if (fabs(velocity_target - vf) < velocity_target_tol) acceleration_trigger = 0;

  if (vf < velocity_target && abs(velocity_target - vf) > velocity_target_tol && acceleration_trigger == 0) acceleration_trigger = 1;

  if (acceleration_trigger != 1) return;

  static double time1 = 0;
  static double time2 = 0.1;
  double time_delta;
  //time2 = high_resolution_time();
  //time2 = t;
  time2 = micros()*1e-6;
  time_delta = time2 - time1;
  if (time_delta < interval) return;
  //if (time_delta < interval) return;
  //time_delta = 0.1;
  time1 = time2;

  static double error = 0;
  static double old_error = 0;
  static double error_dot = 0;
  static double int_error = 0;

  double kp_PID = 30;
  double kd_PID = 0.6;
  double ki_PID = 30;

  double desired_r = 0.2;

  double desired_u_s = u_s;
  static double u_s_old = 0;

  error = (desired_r - r);
  error_dot = (error - old_error) / time_delta;
  int_error = int_error + error * time_delta;

  old_error = error;

  if (is_enable == 1) u_s = kp_PID * error + ki_PID * int_error + kd_PID * error_dot;;


  if (u_s > us_max) u_s = us_max;
  if (u_s < 0) u_s = 0;
}

void brake_PID(bool is_enable, float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval)
{
  static bool deceleration_trigger = 0;

  if (fabs(wb - wf) < velocity_target_tol && velocity_target < vf)  deceleration_trigger = 1;

  if (fabs(velocity_target - vf) < velocity_target_tol) deceleration_trigger = 0;

  if (vf > velocity_target && abs(velocity_target - vf) > velocity_target_tol && deceleration_trigger == 0) deceleration_trigger = 1;

  if (acceleration_trigger == 1 && deceleration_trigger == 0) plot_r_target = 0.2;
  if (acceleration_trigger == 0 && deceleration_trigger == 1) plot_r_target = -0.2;
  if (acceleration_trigger == 0 && deceleration_trigger == 0) plot_r_target = 0;
  
  if (deceleration_trigger != 1) return;

  static double time1 = 0;
  static double time2 = 0.1;
  double time_delta;
  //time2 = t;
  time2 = micros()*1e-6;
  time_delta = time2 - time1;
  if (time_delta < interval) return;
  time1 = time2;

  static double error = 0;
  static double old_error = 0;
  static double error_dot = 0;
  static double int_error = 0;

  double kp_PID = 900;
  double kd_PID = 2;
  double ki_PID = 20;

  double desired_r = -0.2;

  double desired_u_s = u_s;
  static double u_s_old = 0;

  error = (desired_r - r);
  //error = (desired_u_s - u_s_old);
  error_dot = (error - old_error) / time_delta;
  int_error = int_error + error * time_delta;

  old_error = error;

  //if(r > desired_r) u_s = kp_PID * error + ki_PID * int_error + kd_PID * error_dot;
  if (is_enable == 1) u_s = kp_PID * error + ki_PID * int_error + kd_PID * error_dot;

  //if (r > 0.6) u_s = 0;

  //u_s_old = u_s;

  if (u_s > us_max) u_s = us_max;
  if (u_s < 0) u_s = 0;

}
