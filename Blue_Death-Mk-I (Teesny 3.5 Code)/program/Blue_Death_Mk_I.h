#include "LED.h"
#include "HS422.h"
#include "HC05.h"
#include "Ping.h"
#include "Hall_Sensor.h"
#include "Driver.h"

class Blue_Death_Mk_I
{
  private:
  double x, y;
  double dx, dy;
  double ddx, ddy;
  double theta, dtheta, ddtheta;
  double t;

  LED* led;
  HS422* steer;
  HC05* btSerial;
  Ping* ping;
  Hall_Sensor* hall;
  Driver* driver;

  int shared_mem[4];

  public:
  
  Blue_Death_Mk_I(double x, double y, double theta);
  void control(int shared_mem[]);
  void update();
  
  ~Blue_Death_Mk_I();
  
};
