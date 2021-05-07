#include "Blue_Death_Mk_I.h"
#include <Arduino.h>

Blue_Death_Mk_I::Blue_Death_Mk_I(double x, double y, double theta)
{
  this->x = x;
  this->y = y;
  this->theta = theta;
  
  btSerial = new HC05();
  led = new LED(8,7,6);
  steer = new HS422(5);
  ping = new Ping(4);
  hall = new Hall_Sensor(17);
  driver = new Driver(15, 16, 14);
}

void Blue_Death_Mk_I::update()
{
  led->set(40, 15, 100);
  btSerial->update(shared_mem);
  //ping->calculate();
  //ping->get();
  //hall->calculate();
  //hall->get();
  //driver->set(1,0,255);
  //delay(1000);
  //driver->set(1,0,0);
  //delay(1000);
  //steer->update(-50);
  control(shared_mem);
}

void Blue_Death_Mk_I::control(int shared_mem[])
{
  steer->update(shared_mem[0]);
  driver->set(shared_mem[1], shared_mem[2], shared_mem[3]);
  //driver->set(1, 0, 100);
}
Blue_Death_Mk_I::~Blue_Death_Mk_I()
{
  delete
 btSerial;
  delete led;
  delete steer;
  delete ping;
  delete driver;
}
