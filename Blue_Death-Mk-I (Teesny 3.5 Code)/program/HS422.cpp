#include <Arduino.h>
#include "HS422.h"
#include <Servo.h>


HS422::HS422(int pin)
{
  myservo = new Servo;
  myservo->attach(pin);

  offset = 98;
  output = 0;
  limit = 40;
  myservo->write(offset);
}

HS422::update(int servo_write)
{
  if(servo_write > limit) servo_write = limit;
  if(servo_write < -limit) servo_write = -limit; 
  myservo->write(servo_write + offset);
}

HS422::~HS422()
{
  delete myservo;
}
