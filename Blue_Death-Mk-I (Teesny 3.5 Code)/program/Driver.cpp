#include "Driver.h"
#include <Arduino.h>

Driver::Driver(int in1, int in2, int enA)
{
  this->in1_pin = in1;
  this->in2_pin = in2;
  this->enA_pin = enA;

  pinMode(in1_pin, OUTPUT);
  pinMode(in2_pin, OUTPUT);
  pinMode(enA_pin, OUTPUT);
}
void Driver::set(bool in1_val, bool in2_val, int enA_val)
{
  digitalWrite(in1_pin, in1_val);
  digitalWrite(in2_pin, in2_val);
  analogWrite(enA_pin, enA_val);
}
Driver::~Driver() {}
