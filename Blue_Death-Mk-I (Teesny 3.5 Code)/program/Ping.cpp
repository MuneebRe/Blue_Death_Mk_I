#include "Ping.h"
#include <Arduino.h>

Ping::Ping(int pin)
{
  this->pin = pin;
  
  distance = 0;
}
void Ping::calculate()
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);
  distance = 100*(duration / 29.00 / 2.00);
  delay(100);
}
double Ping::get()
{
  Serial.println(distance);
  return distance;
}
Ping::~Ping()
{
  
}
