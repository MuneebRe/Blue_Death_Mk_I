#include <Arduino.h>
#include "LED.h"

LED::LED(int rPin, int gPin, int bPin)
{
  this->rPin = rPin;
  this->gPin = gPin;
  this->bPin = bPin;

  RGB[0] = rPin;
  RGB[1] = gPin;
  RGB[2] = bPin;

  for(int i = 0;i<3;i++)
  {
    pinMode(RGB[i], OUTPUT);
  }

}

void LED::set(int r, int g, int b)
{
  analogWrite(rPin, 256 - r);
  analogWrite(gPin, 256 - g);
  analogWrite(bPin, 256 - b);
}
