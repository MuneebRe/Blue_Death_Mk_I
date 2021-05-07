#include <math.h>

void speed_PID(bool is_enable, float target_vf, float wf, float Rw, float& u_s, float us_max, float t, float interval);

void traction_PID(bool is_enable, float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval);

void brake_PID(bool is_enable, float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval);
