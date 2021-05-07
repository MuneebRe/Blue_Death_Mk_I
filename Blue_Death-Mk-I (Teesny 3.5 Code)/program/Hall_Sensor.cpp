#include "Hall_Sensor.h"
#include <Arduino.h>

Hall_Sensor::Hall_Sensor(int pin)
{
  this->pin = pin;
  pinMode(pin, INPUT);
}

void Hall_Sensor::calculate()
{
  state = digitalRead(pin);
}

void Hall_Sensor::get()
{
  Serial.println(state);
  return rpm;
}

Hall_Sensor::~Hall_Sensor(){}
