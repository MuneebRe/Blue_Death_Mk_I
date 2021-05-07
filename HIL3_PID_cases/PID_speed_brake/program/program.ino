#include <Arduino.h>
#include <math.h>

#include <avr/io.h> // I/O and register definitions
#include <avr/interrupt.h> // interrupt functions and definitions

#include "PinsSetup.h"
#include "pulse.h"

#include "PID.h"

#include "interrupts.h"

void task1();

//float read_ADC_voltage(int channel, int n);

// initial time of test
float t0;

void get_inputs_from_serial(float u[]);

extern float plot_r_target ;
extern float plot_velocity_target;

void setup() 
{	
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

	// I recommend using pins 7 and 8 for the servo pins
	// -- you should avoid using pins 5, 6, and 11 which
	// are used by the car simulator board for analog output.
	
	// you should also avoid using pins 2 and 3 to avoid
	// potential conflict with the simulator board.
	
	//servo1.attach(7); 
	//servo2.attach(8);

  cli();
  
  timer1_setup();
  pin_setup();
  ADC_setup();
  
  sei();

	t0 = micros()*1.0e-6; // initial time (s)

	while(1) 
	{
	  task1();
	}

	delay(1000);
	exit(0); // not using loop()	
}


void task1() 
{		
	int n;
	float y1 = -1, y2 = -1, y3 = -1;
	float wb, wf;
	float u1, u2;
	int pw1, pw2;
	int w1, w2;
  float r;	
	float t;

	const int   PW_MAX = 2000, PW_0 = 1500, PW_MIN = 1000;
	const float wmax = 810.0; // maximum back wheel speed (rad/s)	
	const float V_bat = 12.0; // lipo battery voltage
	const float V_bat_inv = 1/V_bat;	
  const float Rw = 3.2e-2;  //Tire size

	// read clock time (s)
	t = micros()*1.0e-6 - t0;

	// read car outputs ///////////////

	// note that y1 (drive motor), y2 (right front wheel), and 
	// y3 (left front wheel) are the outputs for the car system

	// *** I recommend using pins A1, A3, and A5 for analog inputs ***
	
	//n = 200; // number of ADC samples in averaging filter

	y1 = ADC_read[0];
	y2 = ADC_read[1];
	y3 = ADC_read[2];
	
	// use the following scale for the output y1
	// w = 0 	-> y1 = 2.5 V
	// w = wmax -> y1 = 5.0 V
	// y1 = 2.5 + w * wmax_inv * 2.5 ->
	
	// back wheel angular velocity (rad/s)
	wb = (y1 - 2.5) * 0.4 * wmax;

	// front wheel angular velocity (rad/s)
	//
	wf = (y2 - 2.5) * 0.4 * wmax;
	// calculate inputs

  float tol = 1.0e-10;
  r = (wb*Rw - wf*Rw) / (fabs(wf*Rw) + tol);
  if (r > 10) r = 10;
  if (r < -10) r = 0;

  
  
	// step input for u1 at t = 5s and down to zero at t = 10s
  /*
	if(t > 20) {
		u1 = 0.0;
    //u2 = 0;
	} else {
		u1 = 12.0;
    //u2 = 0;
	}
  */
  
 double velocity_target = 0;

 if (t > 1) velocity_target = 20.00;
 if (t > 10) velocity_target = 0.00;

 speed_PID(1,velocity_target, wf, Rw, u1, V_bat, t, 0.003);
 brake_PID(1,u1, V_bat, r, wf*Rw, wb, wf, velocity_target, t, 0.003);
 traction_PID(0,u1, V_bat, r, wf*Rw, wb, wf, velocity_target, t, 0.003);

 /*
 float u[3+1];
 get_inputs_from_serial(u);
 u1 = u[1];
 u2 = 0;
 */

	// note the maximum input u1 is V_bat (12V)
	// anything more than 12V or less than -12V
	// will have no effect on the simulator
	// use the following if statement to prevent
	// asking for inputs out of range
	if( u1 > V_bat )  u1 = V_bat;
	if( u1 < -V_bat ) u1 = -V_bat;	
	
	//u2 = 0.0;

	// convert inputs to actuator values pw1, pw2

	// convert motor voltage to pulse width
	// u1 = (w1 - PW_0) * PW_R * V_bat ->
	w1 = u1 * V_bat_inv * (PW_MAX - PW_0) + PW_0; 
	
	// saturate input if out of range
	if(w1 > PW_MAX) w1 = PW_MAX;
	if(w1 < PW_MIN) w1 = PW_MIN;	
	
	pw1 = w1;
	
//	if(w2 > PW_MAX) w2 = PW_MAX;
//	if(w2 < PW_MIN) w2 = PW_MIN;

	// set pw2 for testing purposes
	pw2 = 1750;
	
	// set car actuators
	servo_pulse(pw1, pw2);
	
	// print out for testing purposes

  
	Serial.print(t);
	Serial.print(",");

	Serial.print(y1);
	Serial.print(",");
	
	Serial.print(y2);
	Serial.print(",");

	Serial.print(y3);
	Serial.print(",");

  //Serial.print(r);
  //Serial.print(",");  

  //Serial.print(wf*Rw);
  //Serial.print(",");
  
	Serial.print(u1);
	Serial.print(",");	

  Serial.print(u2);
  Serial.print(",");  
	
	Serial.print(wb);
	Serial.print(",");
	
	//Serial.print(wf);
	//Serial.print("\n");	

  Serial.print(wf);
  Serial.print(","); 

  Serial.print(r);
  Serial.print(","); 

  Serial.print(wf*Rw);
  Serial.print(",");

  Serial.print(plot_r_target);
  Serial.print(",");

  Serial.print(plot_velocity_target);
  Serial.print("\n");
	 
	delay(30);
}

/*
float read_ADC_voltage(int channel, int n)
// average = digital low pass filter
// for some value of n it's very good (eg n=200)
{
	int i;
	float input, voltage;
	unsigned long int sum;
	const float ADC_to_V = 1.0/1023.0*5;
	
	sum = 0;
	for(i=0;i<n;i++) {
		sum += analogRead(channel);
	}
	
	input = (float)sum / n; // average analog input
	
	// note that the simple division of float below takes around 
	// 40 us compared to around 4 us for equivalent multiplication
	// -> avoid using float division if possible
	// -> use multiplication instead of division when possible
	// -> avoid putting float division in loops
//	voltage = input / 1023.0 * 5;
	voltage = input * ADC_to_V; // much faster than expression above
	
	return voltage;
}

*/

void loop()
{
	// not used
}

void get_inputs_from_serial(float u[])
   {
     char buffer_in[64];
     Serial.readBytes(buffer_in, 12);
     
     char*p;
     double *pd;
     p = buffer_in;
     
     pd = (double*)p;
     u[1] = *pd;
     p +=sizeof(double);
     
     pd = (double*)p;
     u[2] = *pd;
     p +=sizeof(double);
     
     pd = (double*)p;
     //u[3] = *pd;  Steering angle does not exist, but can be implimented if necessary.

   }
