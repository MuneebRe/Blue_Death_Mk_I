#include <math.h>

void speed_PID(float target_vf, float wf, float Rw, float& u_s, float us_max, float t, float interval);

void traction_PID(float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval);

void brake_PID(float& u_s, float us_max, float r, float vf, float wb, float wf, float velocity_target, float t, float interval);
